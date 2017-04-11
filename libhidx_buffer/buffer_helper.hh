#ifndef PROJECT_BUFFER_HELPER_HH
#define PROJECT_BUFFER_HELPER_HH

#include <string>
#include <stdexcept>
#include <buffer.pb.h>

namespace google {
namespace protobuf {
   class Message;
}
}

namespace libhidx {


    class IOException : public std::runtime_error {
    public:
        IOException(const std::string& str) : std::runtime_error{str} {}
    };

    enum class MessageId : uint8_t {
        init, getDeviceList,
        getDeviceDescriptor, getActiveConfigDescriptor,
        open, close,
        kernelDriverActive, detachKernelDriver, attachKernelDriver,
        claimInterface, releaseInterface,
        getStringDescriptorAscii,
        controlOutTransfer, controlInTransfer,
        interruptOutTransfer, interruptInTransfer
    };



    namespace buffer2 {
        struct Init {
            constexpr static uint8_t id = 1;
        };
    }

    std::string bufferCreate(MessageId messageId, const ::google::protobuf::Message& message);
    std::string bufferCreate(const ::google::protobuf::Message& message);

    std::string recvN(FILE* f, size_t n);
}

#endif //PROJECT_BUFFER_HELPER_HH
