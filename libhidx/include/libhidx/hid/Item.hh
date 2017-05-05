//
// hidviz - Tool for in-depth analysis of USB HID devices communication
// Copyright (C) 2017 Ondrej Budai <ondrej@budai.cz>
//
// This file is part of hidviz.
//
// hidviz is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hidviz is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with hidviz.  If not, see <http://www.gnu.org/licenses/>.
//

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
