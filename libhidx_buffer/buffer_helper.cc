#include "buffer_helper.hh"

#include <google/protobuf/message.h>

std::string libhidx::bufferCreate(libhidx::MessageId messageId, const ::google::protobuf::Message& message){
    std::string buffer;
    auto messageIdStr = std::to_string(static_cast<uint8_t>(messageId));
    messageIdStr.insert(0, 4 - messageIdStr.size(), '0');

    auto messageBuffer = message.SerializeAsString();

    std::string messageLength = std::to_string(messageBuffer.size());
    messageLength.insert(0, 4 - messageLength.size(), '0');

    buffer += messageIdStr;
    buffer += messageLength;
    buffer += messageBuffer;
    buffer += '\n';

    return buffer;
}

std::string libhidx::bufferCreate(const ::google::protobuf::Message& message){
    std::string buffer;
    auto messageBuffer = message.SerializeAsString();

    std::string messageLength = std::to_string(messageBuffer.size());
    messageLength.insert(0, 4 - messageLength.size(), '0');

    buffer += messageLength;
    buffer += messageBuffer;
    buffer += '\n';

    return buffer;
}

std::string libhidx::recvN(FILE* f, size_t n){
    std::vector<char> buffer;
    buffer.resize(n);
    auto count = std::fread(buffer.data(), sizeof(char), n, f);

    if(count != n){
        throw IOException{"Output stream was closed."};
    }

    return {std::begin(buffer), std::end(buffer)};
}
