#ifndef LIBHIDX_COLLECTION_HH
#define LIBHIDX_COLLECTION_HH

#include "Item.hh"

namespace libhidx {
namespace hid {
    class Collection : public Item {

    public:
        explicit Collection(Item* parent = nullptr) : Item{parent} {}

        uint32_t m_type = 0;
        unsigned m_usage = 0;
    };
}
}


#endif //LIBHIDX_COLLECTION_HH
