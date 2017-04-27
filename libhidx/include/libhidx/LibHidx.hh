#ifndef LIBHIDX_HIDLIB_HH
#define LIBHIDX_HIDLIB_HH

#include <libhidx/shared/Utils.hh>

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
