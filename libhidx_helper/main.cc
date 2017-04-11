#include <libusb-1.0/libusb.h>
#include <iostream>
#include <vector>
#include <buffer_helper.hh>
#include <tuple>
#include <map>
#include <buffer.pb.h>
#include <memory>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>

namespace libhidx {

    libusb_context* ctx;
    libusb_device** deviceList;

    std::string recvN(std::istream& stream, size_t n) {
        std::vector<char> buffer;
        buffer.resize(n);
        stream.read(buffer.data(), n);
        if (!stream) {
            throw IOException{"Input stream was closed."};
        }

        return {std::begin(buffer), std::end(buffer)};
    }

    std::pair<MessageId, std::string> receiveMsg(std::istream& stream) {
        constexpr size_t ID_LENGTH = 4;
        auto idStr = recvN(stream, ID_LENGTH);
        auto idNum = std::stol(idStr);

        auto id = static_cast<MessageId>(idNum);

        constexpr size_t LENGTH_LENGTH = 4;
        auto lengthStr = recvN(stream, LENGTH_LENGTH);
        auto length = std::stoul(lengthStr);

        auto message = recvN(stream, length);

        recvN(stream, 1);

        return std::make_pair(id, message);

    }

    void processMessage(const buffer::Init::Request&, buffer::Init::Response& response) {
        auto status = libusb_init(&ctx);
        response.set_retvalue(status);
    }

    void processMessage(const buffer::GetDeviceList::Request&, buffer::GetDeviceList::Response& response) {
        auto status = libusb_get_device_list(ctx, &deviceList);
        response.set_retvalue(status);
        if(status < 0){
            return;
        }

        for(ssize_t i = 0; i < status; ++i){
            auto device = reinterpret_cast<uint64_t>(deviceList[i]);
            response.add_devicelist(device);
        }
    }

    void processMessage(const buffer::GetDeviceDescriptor::Request& request, buffer::GetDeviceDescriptor::Response& response) {
        auto device = reinterpret_cast<libusb_device*>(request.device());
        libusb_device_descriptor desc;
        auto status = libusb_get_device_descriptor(device, &desc);

        auto responseDesc = response.mutable_devicedescriptor();

        response.set_retvalue(status);

        if(status != 0){
            return;
        }

        responseDesc->set_length(desc.bLength);
        responseDesc->set_descriptortype(desc.bDescriptorType);
        responseDesc->set_bcdusb(desc.bcdUSB);
        responseDesc->set_deviceclass(desc.bDeviceClass);
        responseDesc->set_devicesubclass(desc.bDeviceSubClass);
        responseDesc->set_deviceprotocol(desc.bDeviceProtocol);
        responseDesc->set_maxpacketsize(desc.bMaxPacketSize0);
        responseDesc->set_vendor(desc.idVendor);
        responseDesc->set_product(desc.idProduct);
        responseDesc->set_bcddevice(desc.bcdDevice);
        responseDesc->set_indexmanufacturer(desc.iManufacturer);
        responseDesc->set_indexproduct(desc.iProduct);
        responseDesc->set_indexserialnumber(desc.iSerialNumber);
        responseDesc->set_numconfiguration(desc.bNumConfigurations);
    }

    void processMessage(const buffer::GetActiveConfigDescriptor::Request& request, buffer::GetActiveConfigDescriptor::Response& response) {
        auto device = reinterpret_cast<libusb_device*>(request.device());
        libusb_config_descriptor* desc;
        auto status = libusb_get_active_config_descriptor(device, &desc);

        auto responseDesc = response.mutable_configdescriptor();

        response.set_retvalue(status);

        if(status != 0){
            libusb_free_config_descriptor(desc);
            return;
        }

        responseDesc->set_length(desc->bLength);
        responseDesc->set_descriptortype(desc->bDescriptorType);
        responseDesc->set_totallength(desc->wTotalLength);
        responseDesc->set_numinterfaces(desc->bNumInterfaces);
        responseDesc->set_configurationvalue(desc->bConfigurationValue);
        responseDesc->set_indexconfiguration(desc->iConfiguration);
        responseDesc->set_attributes(desc->bmAttributes);
        responseDesc->set_maxpower(desc->MaxPower);
        responseDesc->set_extra(desc->extra, static_cast<size_t>(desc->extra_length));
        responseDesc->set_extralength(desc->extra_length);

        for(unsigned i = 0; i < desc->bNumInterfaces; ++i){
            const auto& interface = desc->interface[i];
            auto responseInterface = responseDesc->add_interface();

            responseInterface->set_numaltsetting(interface.num_altsetting);
            for(int j = 0; j < interface.num_altsetting; ++j){
                const auto& interfaceDesc = interface.altsetting[j];
                auto responseInterfaceDesc = responseInterface->add_altsetting();
                responseInterfaceDesc->set_length(interfaceDesc.bLength);
                responseInterfaceDesc->set_descriptortype(interfaceDesc.bDescriptorType);
                responseInterfaceDesc->set_interfacenumber(interfaceDesc.bInterfaceNumber);
                responseInterfaceDesc->set_alternatesetting(interfaceDesc.bAlternateSetting);
                responseInterfaceDesc->set_numendpoints(interfaceDesc.bNumEndpoints);
                responseInterfaceDesc->set_interfaceclass(interfaceDesc.bInterfaceClass);
                responseInterfaceDesc->set_interfacesubclass(interfaceDesc.bInterfaceSubClass);
                responseInterfaceDesc->set_interfaceprotocol(interfaceDesc.bInterfaceProtocol);
                responseInterfaceDesc->set_indexinterface(interfaceDesc.iInterface);
                responseInterfaceDesc->set_extra(interfaceDesc.extra, static_cast<size_t>(interfaceDesc.extra_length));
                responseInterfaceDesc->set_extralength(interfaceDesc.extra_length);

                for(unsigned k = 0; k < interfaceDesc.bNumEndpoints; ++k){
                    const auto& endpointDesc = interfaceDesc.endpoint[k];
                    auto responseEndpointDesc = responseInterfaceDesc->add_endpoint();
                    responseEndpointDesc->set_length(endpointDesc.bLength);
                    responseEndpointDesc->set_descriptortype(endpointDesc.bDescriptorType);
                    responseEndpointDesc->set_endpointaddress(endpointDesc.bEndpointAddress);
                    responseEndpointDesc->set_attributes(endpointDesc.bmAttributes);
                    responseEndpointDesc->set_maxpacketsize(endpointDesc.wMaxPacketSize);
                    responseEndpointDesc->set_interval(endpointDesc.bInterval);
                    responseEndpointDesc->set_refresh(endpointDesc.bRefresh);
                    responseEndpointDesc->set_synchaddress(endpointDesc.bSynchAddress);
                    responseEndpointDesc->set_extra(endpointDesc.extra, static_cast<size_t>(endpointDesc.extra_length));
                    responseEndpointDesc->set_extralength(endpointDesc.extra_length);
                }
            }
        }

        libusb_free_config_descriptor(desc);
    }

    void processMessage(const buffer::Open::Request& request, buffer::Open::Response& response) {

        libusb_device_handle* handle = nullptr;
        auto status = libusb_open(reinterpret_cast<libusb_device*>(request.device()), &handle);
        response.set_retvalue(status);

        if(status){
            return;
        }
        response.set_handle(reinterpret_cast<uint64_t>(handle));
    }

    void processMessage(const buffer::Close::Request& request, buffer::Close::Response&) {
        libusb_close(reinterpret_cast<libusb_device_handle*>(request.handle()));
    }

    void processMessage(const buffer::KernelDriverActive::Request& request, buffer::KernelDriverActive::Response& response) {
        auto status = libusb_kernel_driver_active(reinterpret_cast<libusb_device_handle*>(request.handle()), request.interfacenumber());
        response.set_retvalue(status);
    }

    void processMessage(const buffer::DetachKernelDriver::Request& request, buffer::DetachKernelDriver::Response& response) {
        auto status = libusb_detach_kernel_driver(reinterpret_cast<libusb_device_handle*>(request.handle()), request.interfacenumber());
        response.set_retvalue(status);
    }

    void processMessage(const buffer::AttachKernelDriver::Request& request, buffer::AttachKernelDriver::Response& response) {
        auto status = libusb_attach_kernel_driver(reinterpret_cast<libusb_device_handle*>(request.handle()), request.interfacenumber());
        response.set_retvalue(status);
    }

    void processMessage(const buffer::ClaimInterface::Request& request, buffer::ClaimInterface::Response& response) {
        auto status = libusb_claim_interface(reinterpret_cast<libusb_device_handle*>(request.handle()), request.interfacenumber());
        response.set_retvalue(status);
    }

    void processMessage(const buffer::GetStringDescriptorAscii::Request& request, buffer::GetStringDescriptorAscii::Response& response) {
        constexpr auto strSize = 1024;
        unsigned char str[strSize];
        auto size = libusb_get_string_descriptor_ascii(
                reinterpret_cast<libusb_device_handle*>(request.handle()),
                static_cast<uint8_t>(request.descindex()),
                str,
                strSize
        );
        response.set_retvalue(size);

        if (size <= 0) {
            return;
        }

        response.set_data(reinterpret_cast<char*>(str), static_cast<size_t>(size));
    }

    void processMessage(const buffer::ReleaseInterface::Request& request, buffer::ReleaseInterface::Response& response) {
        auto status = libusb_release_interface(reinterpret_cast<libusb_device_handle*>(request.handle()), request.interfacenumber());
        response.set_retvalue(status);
    }

    void processMessage(const buffer::ControlOutTransfer::Request& request, buffer::ControlOutTransfer::Response& response) {

        auto status = libusb_control_transfer(
                reinterpret_cast<libusb_device_handle*>(request.handle()),
                static_cast<uint8_t>(request.requesttype()),
                static_cast<uint8_t>(request.request()),
                static_cast<uint16_t>(request.value()),
                static_cast<uint16_t>(request.index()),
                reinterpret_cast<unsigned char*>(const_cast<char*>(request.data().data())),
                static_cast<uint16_t>(request.data().size()),
                request.timeout()
        );
        response.set_retvalue(status);
    }

    void processMessage(const buffer::ControlInTransfer::Request& request, buffer::ControlInTransfer::Response& response) {
        std::vector<unsigned char> buffer;
        buffer.resize(request.length());

        auto size = libusb_control_transfer(
                reinterpret_cast<libusb_device_handle*>(request.handle()),
                static_cast<uint8_t>(request.requesttype()),
                static_cast<uint8_t>(request.request()),
                static_cast<uint16_t>(request.value()),
                static_cast<uint16_t>(request.index()),
                buffer.data(),
                static_cast<uint16_t>(buffer.size()),
                request.timeout()
        );
        response.set_retvalue(size);

        if(size < 0){
            return;
        }

        response.set_data(buffer.data(), static_cast<size_t>(size));
    }

    void processMessage(const buffer::InterruptOutTransfer::Request& request, buffer::InterruptOutTransfer::Response& response) {

        int actualLength;
        auto status = libusb_interrupt_transfer(
                reinterpret_cast<libusb_device_handle*>(request.handle()),
                static_cast<unsigned char>(request.endpoint()),
                reinterpret_cast<unsigned char*>(const_cast<char*>(request.data().data())),
                static_cast<uint16_t>(request.data().size()),
                &actualLength,
                request.timeout()
        );
        response.set_retvalue(status);
        response.set_transferred(actualLength);
    }

    void processMessage(const buffer::InterruptInTransfer::Request& request, buffer::InterruptInTransfer::Response& response) {
        std::vector<unsigned char> buffer;
        buffer.resize(request.length());

        int actualLength;

        auto size = libusb_interrupt_transfer(
                reinterpret_cast<libusb_device_handle*>(request.handle()),
                static_cast<unsigned char>(request.endpoint()),
                buffer.data(),
                static_cast<uint16_t>(buffer.size()),
                &actualLength,
                request.timeout()
        );
        response.set_retvalue(size);
        response.set_transferred(actualLength);
        if(static_cast<size_t>(actualLength) > request.length()){
            response.set_data("");
        } else {
            response.set_data(buffer.data(), static_cast<size_t>(actualLength));
        }

        if(size < 0){
            return;
        }

    }

    template <typename T> std::string parseAndProcessMessage(const std::string& data){
        typename T::Request request;
        request.ParseFromString(data);

        typename T::Response response;
        processMessage(request, response);

        return bufferCreate(response);
    }

    void run() {
//        std::ostream& out = std::cout;
//        std::istream& in = std::cin;
        std::ofstream out{"/tmp/fromhelper"};
        std::ifstream in{"/tmp/tohelper"};

        while (true) {

            MessageId messageId;
            std::string message;

            try {
                std::tie(messageId, message) = receiveMsg(in);
            } catch (std::runtime_error& ex) {
                break;
            }

            const std::map<MessageId, std::string (*)(const std::string& msg)> functions = {
                    {MessageId::init, parseAndProcessMessage<buffer::Init>},
                    {MessageId::getDeviceList, parseAndProcessMessage<buffer::GetDeviceList>},
                    {MessageId::getDeviceDescriptor, parseAndProcessMessage<buffer::GetDeviceDescriptor>},
                    {MessageId::getActiveConfigDescriptor, parseAndProcessMessage<buffer::GetActiveConfigDescriptor>},
                    {MessageId::open, parseAndProcessMessage<buffer::Open>},
                    {MessageId::close, parseAndProcessMessage<buffer::Close>},
                    {MessageId::kernelDriverActive, parseAndProcessMessage<buffer::KernelDriverActive>},
                    {MessageId::detachKernelDriver, parseAndProcessMessage<buffer::DetachKernelDriver>},
                    {MessageId::attachKernelDriver, parseAndProcessMessage<buffer::AttachKernelDriver>},
                    {MessageId::claimInterface, parseAndProcessMessage<buffer::ClaimInterface>},
                    {MessageId::releaseInterface, parseAndProcessMessage<buffer::ReleaseInterface>},
                    {MessageId::getStringDescriptorAscii, parseAndProcessMessage<buffer::GetStringDescriptorAscii>},
                    {MessageId::controlOutTransfer, parseAndProcessMessage<buffer::ControlOutTransfer>},
                    {MessageId::controlInTransfer, parseAndProcessMessage<buffer::ControlInTransfer>},
                    {MessageId::interruptOutTransfer, parseAndProcessMessage<buffer::InterruptOutTransfer>},
                    {MessageId::interruptInTransfer, parseAndProcessMessage<buffer::InterruptInTransfer>},
            };

            auto response = functions.at(messageId)(message);
            out << response << std::flush;

        }

        libusb_exit(ctx);
    }
}

int main(){
    libhidx::run();
    return 0;
}
