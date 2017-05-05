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

#ifndef PROJECT_CONNECTOR_HH
#define PROJECT_CONNECTOR_HH

#include <libhidx/server/Utils.hh>

#include <asio.hpp>

#include <memory>

namespace subprocess {
    class Popen;
}

namespace libhidx {

    class Connector {
    public:
        virtual ~Connector() = default;
        virtual bool connect() = 0;
        virtual std::string sendMessage(const std::string& message) = 0;
    };

    class LocalConnector : public Connector {
    public:
        LocalConnector();
        bool connect() override;
        std::string sendMessage(const std::string& message) override;
    };

#ifdef __linux__
    class UnixSocketConnector : public Connector {
    public:
        UnixSocketConnector();
        ~UnixSocketConnector() override;

        bool connect() override;
        std::string sendMessage(const std::string& message) override;
    private:
        std::string getExecutablePath();

        std::unique_ptr<subprocess::Popen> m_process;
        std::string m_socketDir;
        asio::io_service m_ioService;
        std::unique_ptr<asio::generic::stream_protocol::socket> m_socket;
        std::string getServerPath();
    };
#endif

}


#endif //PROJECT_CONNECTOR_HH
