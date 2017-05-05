#ifndef PROJECT_SERVER_HH_HH
#define PROJECT_SERVER_HH_HH

#include <libhidx/server/Utils.hh>

#include <string>

namespace libhidx {
namespace server {
    void run(std::string argc, bool watchParent);
    std::string cmd(const std::string& request);

}
}

#endif //PROJECT_SERVER_HH_HH
