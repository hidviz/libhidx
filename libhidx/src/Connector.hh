#ifndef PROJECT_CONNECTOR_HH
#define PROJECT_CONNECTOR_HH

#include <libhidx/shared/Utils.hh>

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
    };
#endif

}


#endif //PROJECT_CONNECTOR_HH
