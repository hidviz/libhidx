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

#ifndef HIDVIZ_USAGE_H
#define HIDVIZ_USAGE_H

#include <cstdint>
#include <string>

namespace libhidx {
namespace hid {

    class Control;

    class Usage {
    public:
        Usage(unsigned usageId, Control* control);

        auto getId() const { return m_id;}
        const auto &getName() const { return m_name; }

        auto getLogicalValue() const { return m_logicalValue; }
        void setLogicalValue(uint32_t rawLogicalValue);
        bool setLogicalValueFromUser(const std::string& dataString);
        auto getPhysicalValue() const {return m_physicalValue;}
        const auto& getControl() const {return *m_control;}


    private:
        unsigned m_id = 0;
        std::string m_name;
        Control* m_control = nullptr;

        int64_t m_logicalValue = 0;
        double m_physicalValue = 0;

        double logicalToPhysical(int64_t);
        //TODO uint32_t physicalToLogical(double);
        int64_t convertLogicalValue(uint32_t value, unsigned int size);
    };
}
}


#endif //HIDVIZ_USAGE_H
