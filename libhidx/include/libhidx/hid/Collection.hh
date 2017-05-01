#ifndef LIBHIDX_COLLECTION_HH
#define LIBHIDX_COLLECTION_HH

#include "Item.hh"

namespace libhidx {
    class Parser;
namespace hid {
    class Collection : public Item {
    friend class libhidx::Parser;
    public:
        explicit Collection(Item* parent = nullptr) : Item{parent} {}
        auto getType(){return m_type;}
        std::string getTypeStr();
        auto getUsage(){return m_usage;}
        std::string getUsageStr();

    private:
        uint32_t m_type = 0;
        unsigned m_usage = 0;
    };
}
}


#endif //LIBHIDX_COLLECTION_HH
