#include <libhidx/LibHidx.hh>

#include <libhidx/Device.hh>

#ifndef _WIN32
#include "subprocess.hh"
#endif

#include <climits>
#include <fstream>


namespace libhidx {

#ifndef _WIN32
    std::string getexepath()
    {
        char result[ PATH_MAX ];
        ssize_t count = readlink( "/proc/self/cwd", result, PATH_MAX );
        return std::string( result, (count > 0) ? count : 0 );
    }
#endif

    LibHidx::LibHidx() {

#ifndef _WIN32
        m_process = std::make_unique<subprocess::Popen>(
                "pkexec " + getexepath() + "/../libhidx/libhidx_server_daemon/libhidx_server_daemon",
                subprocess::input{subprocess::PIPE},
                subprocess::output{subprocess::PIPE}
        );
        // this is weird, but it works
        m_input = m_process->output();
        m_output = m_process->input();
#endif
//        m_input = fopen("/tmp/fromhelper", "r");
//        m_output = fopen("/tmp/tohelper", "w");

        buffer::Init::Request initReq;

        auto response = sendMessage<buffer::Init>(MessageId::init, initReq);

        if(response.retvalue() != 0){
            throw LibHidxError{"Cannot initialize libhidx."};
        }

        m_ctx = response.ctx();
    }

    LibHidx::~LibHidx() {
        freeDevices();
#ifndef _WIN32
        m_process->kill();
#endif
        // TODO call exit
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

}
