#include <libhidx/LibHidx.hh>

#include <libhidx/server/Buffer.hh>
#include <libhidx/Device.hh>
#include <Connector.hh>

#include <array>



namespace libhidx {

#ifndef _WIN32

#endif

    LibHidx::LibHidx() {}

    void LibHidx::connectLocal(){
        m_connector = std::make_unique<LocalConnector>();
    }

#ifdef __linux__
    void LibHidx::connectUnixSocket(){
        m_connector = std::make_unique<UnixSocketConnector>();
    }
#endif

    bool LibHidx::doConnect() {
        try {
            m_connector->connect();
        } catch(asio::system_error&){
            return false;
        }

        return true;
    }

    void LibHidx::init(){
        buffer::Init::Request initReq;

        auto response = sendMessage<buffer::Init>(MessageId::init, initReq);

        if(response.retvalue() != 0){
            throw LibHidxError{"Cannot initialize libhidx."};
        }

        m_ctx = response.ctx();
        m_initialized = true;
    }

    LibHidx::~LibHidx() {
        freeDevices();
        if(!m_initialized){
            return;
        }
        buffer::Exit::Request exitReq;
        exitReq.set_ctx(m_ctx);
        sendMessage<buffer::Exit>(MessageId::exit, exitReq);
    }

    void LibHidx::reloadDevices() {
        freeDevices();

        buffer::GetDeviceList::Request getDeviceListRequest;
        getDeviceListRequest.set_ctx(m_ctx);

        auto response = sendMessage<buffer::GetDeviceList>(MessageId::getDeviceList, getDeviceListRequest);

        for(const auto& deviceHandle: response.devicelist()){
            m_devices.emplace_back(std::make_unique<Device>(deviceHandle, *this));
        }

        m_listHandle = response.listhandle();
    }

    void LibHidx::loadDevices() {
        if(!m_listHandle){
            reloadDevices();
        }
    }

    void LibHidx::freeDevices()  {
        m_devices.clear();
        if (m_listHandle) {
            buffer::FreeDeviceList::Request freeDeviceListReq;
            freeDeviceListReq.set_listhandle(m_listHandle);
            freeDeviceListReq.set_unrefdevices(1);
            sendMessage<buffer::FreeDeviceList>(MessageId::freeDeviceList, freeDeviceListReq);
            m_listHandle = 0;
        }
    }

    std::string LibHidx::sendMessage(const std::string& message) {
        return m_connector->sendMessage(message);
    }

}
