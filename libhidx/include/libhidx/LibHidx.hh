#ifndef LIBHIDX_HIDLIB_HH
#define LIBHIDX_HIDLIB_HH

#include <buffer_helper.hh>

#include <libusb-1.0/libusb.h>

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
        LibHidxError(const std::string& str) : std::runtime_error{str}{}
    };

    class Device;

    class LibHidx {
    public:
        LibHidx();
        LibHidx(const LibHidx&) = delete;
        ~LibHidx();

        void loadDevices();
        const auto& getDevices(){return m_devices;}

//        void sendMessage(MessageId messageId, const ::google::protobuf::Message& msg);
//        template <typename T>
//        T receiveResponse() {
//            constexpr size_t LENGTH_LENGTH = 4;
//            auto lengthStr = recvN(m_input, LENGTH_LENGTH);
//            auto length = std::stoul(lengthStr);
//
//            auto responseStr = recvN(m_input, length);
//            T response;
//            response.ParseFromString(responseStr);
//
//            recvN(m_input, 1);
//
//            return response;
//        }

        template<typename Msg>
        void sendMessage(MessageId messageId,
                         std::function<void(typename Msg::Request&)> requestFunction,
                         std::function<void(const typename Msg::Response&)> responseFunction
        ){
            typename Msg::Request request;
            requestFunction(request);
            auto response = sendMessage<Msg>(messageId, request);
            responseFunction(response);
        }

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

    private:
        std::vector<std::unique_ptr<Device>> m_devices;
        std::unique_ptr<subprocess::Popen> m_process;
        FILE* m_input;
        FILE* m_output;
        std::mutex m_commMutex;
    };

}


#endif //LIBHIDX_HIDLIB_HH
