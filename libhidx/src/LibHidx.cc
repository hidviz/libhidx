#include <libhidx/LibHidx.hh>

#include <libhidx/Device.hh>

#include <buffer.pb.h>
#include <buffer_helper.hh>

#include "subprocess.hh"

#include <fstream>


namespace libhidx {

    std::string getexepath()
    {
        char result[ PATH_MAX ];
        ssize_t count = readlink( "/proc/self/cwd", result, PATH_MAX );
        return std::string( result, (count > 0) ? count : 0 );
    }



    LibHidx::LibHidx() {

        m_process = std::make_unique<subprocess::Popen>(
                "pkexec " + getexepath() + "/../libhidx/libhidx_helper/libhidx_helper",
                subprocess::input{subprocess::PIPE},
                subprocess::output{subprocess::PIPE}
        );
        // this is weird, but it works
//        m_input = m_process->output();
//        m_output = m_process->input();

        m_input = fopen("/tmp/fromhelper", "r");
        m_output = fopen("/tmp/tohelper", "w");

        buffer::Init::Request initReq;

        auto response = sendMessage<buffer::Init>(MessageId::init, initReq);

        if(response.retvalue() != 0){
            throw LibHidxError{"Cannot initialize libhidx."};
        }
    }

    LibHidx::~LibHidx() {
        m_devices.clear();
//TODO        libusb_free_device_list(m_deviceList, 0);
        m_process->kill();
        // TODO call exit
    }

    void LibHidx::loadDevices() {
        // TODO free device list
        auto response = sendMessage<buffer::GetDeviceList>(MessageId::getDeviceList, {});

        for(const auto& deviceHandle: response.devicelist()){
            m_devices.emplace_back(std::make_unique<Device>(deviceHandle, *this));
        }
    }

}
