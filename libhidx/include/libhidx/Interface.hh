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

#ifndef LIBHIDX_INTERFACE_HH
#define LIBHIDX_INTERFACE_HH

#include <atomic>
#include <thread>
#include <functional>
#include <vector>

namespace libhidx {

    namespace buffer {
        class Interface;
        class InterfaceDescriptor;
    };

    namespace hid {
        class Item;
    }

    class Device;
    class InterfaceHandle;

    class Interface {
    public:
        Interface(const buffer::Interface&, Device& device);
        Interface(const Interface&) = delete;
        ~Interface();

        bool isHid() const;
        uint32_t getNumber() const;
        auto& getDevice() {return m_device;}
        std::string getName() const;
        std::shared_ptr<InterfaceHandle> getHandle();
        const auto& getDesc() const {return m_interface;}
        hid::Item& getParsedHidReportDesc();
        const std::string& getRawHidReportDesc();
        void setReadingListener(std::function<void()>);
        void sendData();


        void beginReading();
        void stopReading();

    private:
        const buffer::InterfaceDescriptor& m_interface;
        Device& m_device;
        std::weak_ptr<InterfaceHandle> m_handle;

        std::thread readingThread;
        std::atomic_bool readingRuns{false};
        std::atomic_bool stopReadingRequest{false};
        void runner();

        uint32_t m_inputAddress = 0;
        uint32_t m_inputMaxSize = 0;
        uint32_t m_outputAddress = 0;
        bool m_hasOutputAddress = false;

        std::function<void()> m_listener;

        bool m_hidParsed = false;
        std::unique_ptr<hid::Item> m_parsedHidReportDesc;
        std::string m_rawHidReportDesc;

        void updateData(std::vector<unsigned char>&& data);
        void sendOutputReport(const std::vector<unsigned char>& data);
        void parseHidReportDesc();
    };

}


#endif //LIBHIDX_INTERFACE_HH
