#include <libhidx/Device.hh>

#include <libhidx/InterfaceHandle.hh>
#include <libhidx/LibHidx.hh>

namespace libhidx {

    Device::Device(uint64_t device, LibHidx& lib) : m_lib{lib} {
        m_device = device;

        buffer::GetDeviceDescriptor::Request msg;
        msg.set_device(m_device);
        auto response = m_lib.sendMessage<buffer::GetDeviceDescriptor>(MessageId::getDeviceDescriptor, msg);

        if(response.retvalue() != 0){
            throw IOException{"Cannot retrieve device descriptor."};
        }

        m_descriptor = std::make_unique<const buffer::DeviceDescriptor>(response.devicedescriptor());

        buffer::GetActiveConfigDescriptor::Request msg2;
        msg2.set_device(m_device);
        auto response2 = m_lib.sendMessage<buffer::GetActiveConfigDescriptor>(MessageId::getActiveConfigDescriptor, msg2);
        if(response.retvalue() != 0){
            throw IOException{"Cannot retrieve config descriptor."};
        }

        m_config_descriptor = std::make_unique<const buffer::ConfigDescriptor>(response2.configdescriptor());
        fillInterfaces();
    }

    const DeviceStrings& Device::getStrings() {
        if(!m_strings) {
            try {
              auto deviceHandle = m_interfaces.front()->getHandle();
              m_strings = std::make_unique<DeviceStrings>(deviceHandle->readStrings());
            } catch(ConnectionException &ex){
              m_strings = std::make_unique<DeviceStrings>();
            }
        }

        return *m_strings;
    }

    void Device::fillInterfaces() {
        for(const auto& interfaceDesc: m_config_descriptor->interface()){
            m_interfaces.emplace_back(std::make_unique<Interface>(interfaceDesc, *this));
        }
    }
}
