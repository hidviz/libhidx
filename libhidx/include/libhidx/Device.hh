#ifndef LIBHIDX_HIDLIBDEVICE_HH
#define LIBHIDX_HIDLIBDEVICE_HH

#include "DeviceStrings.hh"
#include "Interface.hh"

#include <libusb-1.0/libusb.h>

#include <vector>
#include <memory>

namespace libhidx {
    namespace buffer {
        class DeviceDescriptor;
        class ConfigDescriptor;
    }
    class Interface;
    class LibHidx;

    class Device {
    public:

        Device(uint64_t handle, LibHidx& lib);
        Device(const Device&) = delete;

        const auto& getPtr() const { return m_device; }
        auto getDesc() const { return m_descriptor.get(); }
        const DeviceStrings& getStrings();
        const auto& getInterfaces() const {return m_interfaces;}
        auto& getLib() {return m_lib;}
        const auto getConfigDesc() const {return m_config_descriptor.get();}


    private:
        uint64_t m_device = 0;
        std::unique_ptr<const buffer::DeviceDescriptor> m_descriptor;
        std::unique_ptr<const buffer::ConfigDescriptor> m_config_descriptor;
        std::unique_ptr<DeviceStrings> m_strings;
        std::vector<std::unique_ptr<Interface>> m_interfaces;
        LibHidx& m_lib;

        void fillInterfaces();

    };
}


#endif //LIBHIDX_HIDLIBDEVICE_HH
