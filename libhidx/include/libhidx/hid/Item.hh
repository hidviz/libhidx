#ifndef LIBHIDX_ITEM_HH
#define LIBHIDX_ITEM_HH

#include <vector>
#include <memory>

namespace libhidx {
    class Parser;
namespace hid {

    class Item {
        friend class libhidx::Parser;
    public:
        Item(Item* parent = nullptr) : m_parent{parent}{}
        Item(const Item&) = delete;
        Item& operator= (const Item&) = delete;
        virtual ~Item(){}

        void appendChild(Item* child);
        Item* child(int row);
        size_t childCount() const;
        Item* parentItem();
        size_t row() const;
        void forEach(std::function<void(Item*)>);
        auto isNumbered(){return topItem()->m_numbered;}

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
