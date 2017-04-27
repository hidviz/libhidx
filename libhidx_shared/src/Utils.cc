#include <libhidx/shared/Utils.hh>

#include <google/protobuf/message.h>

using generic = asio::generic::stream_protocol;

constexpr size_t LENGTH_LENGTH = 8;
constexpr size_t LENGTH_ID = 3;

namespace libhidx {
namespace utils {

    std::string readFromSocket(generic::socket& socket, const size_t length) {
        asio::streambuf buffer;
        auto received = asio::read(socket, buffer.prepare(length));
        if (received != length) {
            //TODO
            throw;
        }
        buffer.commit(length);
        return {asio::buffers_begin(buffer.data()), asio::buffers_end(buffer.data())};
    }

    std::string readMessage(generic::socket& stream) {
        auto lengthStr = readFromSocket(stream, LENGTH_LENGTH);
        auto length = std::stoul(lengthStr);

        auto message = readFromSocket(stream, length);

        return message;

    }
    void writeMessage(generic::socket& socket, const std::string& message){
        auto length = std::to_string(message.length());

        length.insert(0, LENGTH_LENGTH - length.length(), '0');

        asio::write(socket, asio::buffer(length));
        asio::write(socket, asio::buffer(message));
    }

    std::string packMessage(MessageId messageId, const std::string& payload){
        auto id = static_cast<unsigned>(messageId);
        auto idStr = std::to_string(id);

        idStr.insert(0, LENGTH_ID - idStr.length(), '0');

        return idStr + payload;
    }

    std::pair<MessageId, std::string> unpackMessage(const std::string& message){
        std::string idStr{std::begin(message), std::begin(message) + LENGTH_ID};

        auto id = std::stoul(idStr);
        auto messageId = static_cast<MessageId>(id);

        return {messageId, {std::begin(message) + LENGTH_ID, std::end(message)}};
    };
}}
