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

#ifndef LIBHIDX_HIDLIB_HH
#define LIBHIDX_HIDLIB_HH

#include <libhidx/server/Utils.hh>

#include <vector>
#include <memory>
#include <mutex>
#include <functional>

namespace google {
namespace protobuf {
    class Message;
}
}

namespace libhidx {


    class LibHidxError : public std::runtime_error {
    public:
        explicit LibHidxError(const std::string& str) : std::runtime_error{str}{}
    };

    class Device;
    class Connector;

    class LibHidx {
    public:
        LibHidx();
        LibHidx(const LibHidx&) = delete;
        ~LibHidx();

        void connectLocal();
#ifdef __linux__
        void connectUnixSocket();
#endif

        bool doConnect();

        void init();

        void reloadDevices();
        void loadDevices();
        void freeDevices();
        const auto& getDevices(){return m_devices;}

        template<typename Msg>
        typename Msg::Response sendMessage(MessageId messageId, const typename Msg::Request& request){
            std::lock_guard<std::mutex> lock{m_commMutex};

            auto messageOut = utils::packMessage(messageId, request.SerializeAsString());

            auto messageIn = sendMessage(messageOut);

            auto payloadIn = utils::unpackMessage(messageIn);

            typename Msg::Response response;

            response.ParseFromString(payloadIn.second);

            return response;
        }


    private:
        std::string sendMessage(const std::string& message);

        std::unique_ptr<Connector> m_connector;

        std::vector<std::unique_ptr<Device>> m_devices;
        std::mutex m_commMutex;
        uint64_t m_listHandle = 0;
        uint64_t m_ctx = 0;
        bool m_initialized = false;
    };

}

#endif //LIBHIDX_HIDLIB_HH
