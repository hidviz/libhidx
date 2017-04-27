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
