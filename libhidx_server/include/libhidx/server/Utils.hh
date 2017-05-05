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

#ifndef PROJECT_BUFFER_HELPER_HH
#define PROJECT_BUFFER_HELPER_HH

#include <string>
#include <stdexcept>
#include <asio.hpp>

namespace libhidx {
    class IOException : public std::runtime_error {
    public:
        explicit IOException(const std::string& str) : std::runtime_error{str} {}
    };

    enum class MessageId : uint8_t {
        init, exit, getDeviceList, freeDeviceList,
        getDeviceDescriptor, getActiveConfigDescriptor,
        open, close,
        kernelDriverActive, detachKernelDriver, attachKernelDriver,
        claimInterface, releaseInterface,
        getStringDescriptorAscii,
        controlOutTransfer, controlInTransfer,
        interruptOutTransfer, interruptInTransfer
    };

    constexpr const char* SOCKET_FILENAME = "sock";

namespace utils {

    std::string readMessage(asio::generic::stream_protocol::socket& socket);
    void writeMessage(asio::generic::stream_protocol::socket& socket, const std::string& message);
    std::string packMessage(MessageId messageId, const std::string& payload);
    std::pair<MessageId, std::string> unpackMessage(const std::string& message);
}}

#endif //PROJECT_BUFFER_HELPER_HH
