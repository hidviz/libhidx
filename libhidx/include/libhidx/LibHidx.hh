#ifndef LIBHIDX_HIDLIB_HH
#define LIBHIDX_HIDLIB_HH

#include <buffer_helper.hh>
#include <libhidx/server/Server.hh>

#include <vector>
#include <memory>
#include <mutex>
#include <functional>



namespace subprocess {
    class Popen;
}

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

    class LibHidx {
    public:
        LibHidx();
        LibHidx(const LibHidx&) = delete;
        ~LibHidx();

        void reloadDevices();
        void loadDevices();
        void freeDevices();
        const auto& getDevices(){return m_devices;}

#ifndef _WIN32 //Linux
        template<typename Msg>
        typename Msg::Response sendMessage(MessageId messageId, const typename Msg::Request& request){
            std::lock_guard<std::mutex> lock{m_commMutex};

            auto buffer = bufferCreate(messageId, request);

            auto messageSize = buffer.size();

            auto size = std::fwrite(buffer.data(), sizeof(char), messageSize, m_output);
            std::fflush(m_output);

            if(size != messageSize){
                throw IOException{"Cannot send message."};
            }

            constexpr size_t LENGTH_LENGTH = 4;
            auto lengthStr = recvN(m_input, LENGTH_LENGTH);
            auto length = std::stoul(lengthStr);

            auto responseStr = recvN(m_input, length);
            typename Msg::Response response;
            response.ParseFromString(responseStr);

            recvN(m_input, 1);

            return response;
        }
#else // Windows

        template<typename Msg>
        typename Msg::Response sendMessage(MessageId messageId, const typename Msg::Request& request){
            std::lock_guard<std::mutex> lock{m_commMutex};

            auto buffer = request.SerializeAsString();

            auto responseStr = server::cmd(messageId, buffer);

            typename Msg::Response response;
            response.ParseFromString(responseStr);

            return response;
        }
#endif


    private:
        std::vector<std::unique_ptr<Device>> m_devices;
#ifndef _WIN32
        std::unique_ptr<subprocess::Popen> m_process;
#endif
        FILE* m_input;
        FILE* m_output;
        std::mutex m_commMutex;
        uint64_t m_listHandle = 0;
        uint64_t m_ctx = 0;
    };

}

#endif //LIBHIDX_HIDLIB_HH
