#ifndef LIBHIDX_HIDLIBDEVICEHANDLE_HH
#define LIBHIDX_HIDLIBDEVICEHANDLE_HH


#include "Device.hh"
#include "DeviceStrings.hh"

#include <libhidx/server/Buffer.hh>

namespace libhidx {

    class ConnectionException : public std::runtime_error {
    public:
        explicit ConnectionException(const std::string& err) : std::runtime_error{err}{}
    };

    class InterfaceHandle {
    public:
        ~InterfaceHandle();
        DeviceStrings readStrings();

        int controlOutTransfer(uint8_t requestType,
                               uint8_t request,
                               uint16_t value,
                               uint16_t index,
                               const void* data,
                               size_t length,
                               unsigned int timeout
        );

        std::pair<int, std::string> controlInTransfer(uint8_t requestType,
                                                      uint8_t request,
                                                      uint16_t value,
                                                      uint16_t index,
                                                      uint16_t length,
                                                      unsigned int timeout
        );

        buffer::InterruptOutTransfer::Response
        interruptOutTransfer(unsigned char endpoint, const void* data,
                                              size_t length,
                                              unsigned timeout);
        buffer::InterruptInTransfer::Response
        interruptInTransfer(unsigned char endpoint, uint16_t length, unsigned timeout);

        friend class Interface;

    private:
        explicit InterfaceHandle(Interface& interface);
        Interface& m_interface;
        uint32_t m_ifaceNumber;

        std::string extractString(uint32_t index) const;

        uint64_t m_handle;
        LibHidx& m_lib;
    };

}


#endif //LIBHIDX_HIDLIBDEVICEHANDLE_HH
