#ifndef PROJECT_SERVER_HH_HH
#define PROJECT_SERVER_HH_HH

#include <string>
#include <buffer_helper.hh>

namespace libhidx {
namespace server {
    void run();
    std::string cmd(MessageId messageId, const std::string& request);
    std::string createResponse(const std::string& response);

}
}

#endif //PROJECT_SERVER_HH_HH
