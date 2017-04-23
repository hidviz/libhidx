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
