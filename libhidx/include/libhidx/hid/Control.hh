#ifndef LIBHIDX_CONTROL_HH
#define LIBHIDX_CONTROL_HH

#include <mutex>
#include "Item.hh"
#include "Usage.hh"

namespace libhidx {
    class Parser;
namespace hid {
    class Control : public Item {
        friend class libhidx::Parser;

    public:
        explicit Control(Item* parent = nullptr) : Item{parent} {}
        void setData(const std::vector<unsigned char>& rawData, unsigned reportId);
        uint32_t getData() const;


        enum class Type {
            UNDEFINED = -1,
            INPUT = 0,
            OUTPUT  = 1,
            FEATURE = 2
        };

        auto getReportType() const {return m_reportType;}
        const auto& getUsages() const {return m_usages;}
        auto getOffset() const {return m_offset;};
        auto getSize() const {return m_reportSize;}
        auto getCount() const {return m_reportCount;}
        auto getLogicalMinimum() const {return m_logicalMinimum;}
        auto getLogicalMaximum() const {return m_logicalMaximum;}
        auto getPhysicalMinimum() const {return m_physicalMinimum;}
        auto getPhysicalMaximum() const {return m_physicalMaximum;}
        auto getUnitExponent() const {return m_unitExponent;}
        auto getReportId() const {return m_reportId;}
        auto getUnit() const {return m_unit;}
        auto getReportSize() const {return m_reportSize;}
        auto getReportCount() const {return m_reportCount;}

        enum Flag {
            CONSTANT = 0x01,
            VARIABLE = 0x02,
            // do not use word RELATIVE, as it's defined as macro in msys2 env
            FRELATIVE = 0x04,
            WRAP = 0x08,
            NONLINEAR = 0x10,
            NO_PREFERRED = 0x20,
            NULL_STATE = 0x40,
            VOLATILE = 0x80,
        };
        bool isConstant() const {return (m_flags & Flag::CONSTANT) != 0;}
        bool isVariable() const {return (m_flags & Flag::VARIABLE) != 0;}
        bool isRelative() const {return (m_flags & Flag::FRELATIVE) != 0;}
        bool isWrap() const {return (m_flags & Flag::WRAP) != 0;}
        bool isNonlinear() const {return (m_flags & Flag::NONLINEAR) != 0;}
        bool isNoPreferred() const {return (m_flags & Flag::NO_PREFERRED) != 0;}
        bool isNullState() const {return (m_flags & Flag::NULL_STATE) != 0;}
        bool isVolatile() const {return (m_flags & Flag::VOLATILE) != 0;}
        bool isBinary() const;

    private:
        std::size_t m_offset = 0;
        std::vector<std::unique_ptr<Usage>> m_usages;
        uint32_t m_flags = 0;
        Type m_reportType = Type::UNDEFINED;
        unsigned m_reportSize = 0;
        unsigned m_reportCount = 0;
        int32_t m_logicalMinimum = 0;
        int32_t m_logicalMaximum = 0;
        int32_t m_physicalMinimum = 0;
        int32_t m_physicalMaximum = 0;
        int32_t m_unitExponent = 0;
        unsigned m_unit = 0;
        unsigned int m_reportId = 0;

        std::vector<bool> extractData(const std::vector<unsigned char>& vector);

        uint32_t extractVariableUsageData(const std::vector<bool>& data, unsigned index);

        Usage* findUsageByValue(uint32_t value);
    };
}
}


#endif //LIBHIDX_CONTROL__HH
