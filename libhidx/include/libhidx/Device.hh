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

#ifndef LIBHIDX_HIDLIBDEVICE_HH
#define LIBHIDX_HIDLIBDEVICE_HH

#include <vector>
#include <memory>

namespace libhidx {
    namespace buffer {
        class DeviceDescriptor;
        class ConfigDescriptor;
    }
    class Interface;
    class LibHidx;
    class DeviceStrings;

    class Device {
    public:

        Device(uint64_t handle, LibHidx& lib);
        Device(const Device&) = delete;

        const auto& getPtr() const { return m_device; }
        auto getDesc() const { return m_descriptor.get(); }
        const DeviceStrings& getStrings();
        const auto& getInterfaces() const {return m_interfaces;}
        auto& getLib() {return m_lib;}
        auto getConfigDesc() const {return m_config_descriptor.get();}


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
