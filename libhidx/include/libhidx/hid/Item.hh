#ifndef LIBHIDX_ITEM_HH
#define LIBHIDX_ITEM_HH

#include <vector>
#include <memory>
#include <functional>

namespace libhidx {
    class Parser;
namespace hid {

    class Item {
        friend class libhidx::Parser;
    public:
        explicit Item(Item* parent = nullptr) : m_parent{parent}{}
        Item(const Item&) = delete;
        Item& operator= (const Item&) = delete;
        virtual ~Item() = default;

        void appendChild(Item* child);
        Item* child(int row);
        size_t childCount() const;
        Item* parentItem();
        size_t row() const;
        void forEach(const std::function<void(Item*)>& function);
        auto isNumbered(){return topItem()->m_numbered;}
        unsigned getLevel(){if(!m_parent){return 0;} return m_parent->getLevel() + 1;}

    protected:
        Item* topItem();

    private:
        std::vector<std::unique_ptr<Item>> m_children;
        Item* m_parent = nullptr;
        bool m_numbered = false;

    };
}
}


#endif //LIBHIDX_ITEM_HH
