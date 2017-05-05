//
// hidviz - Tool for in-depth analysis of USB HID devices communication
// Copyright (C) 2017 Ondrej Budai <ondrej@budai.cz>
//
// This file is part of hidviz.
//
// hidviz is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hidviz is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with hidviz.  If not, see <http://www.gnu.org/licenses/>.
//

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
