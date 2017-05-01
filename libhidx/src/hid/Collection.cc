#include <libhidx/hid/Collection.hh>
#include <libhidx/Usages.hh>

namespace libhidx {
namespace hid {
    std::string Collection::getTypeStr() {
        static const char* types[] = {
            "Physical", "Application", "Logical", "Report", "Named Array",
            "Usage Switch", "Usage modifier"
        };

        if(m_type >= (sizeof(types) / sizeof(*types))){
            return "Unknown";
        }

        return types[m_type];
    }

    std::string hid::Collection::getUsageStr() {
        return getHidUsageText(m_usage);
    }
}
}
