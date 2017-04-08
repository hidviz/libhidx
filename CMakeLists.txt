cmake_minimum_required(VERSION 3.2)
project(libhidx VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(libusb-1.0 REQUIRED)
add_library(libusb-1.0 SHARED IMPORTED)
set_property(TARGET libusb-1.0 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBUSB_1_INCLUDE_DIRS})
set_property(TARGET libusb-1.0 PROPERTY IMPORTED_LOCATION ${LIBUSB_1_LIBRARY})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(hidx
        src/LibHidx.cc
        src/Device.cc
        src/InterfaceHandle.cc
        src/Interface.cc
        src/Parser.cc
        src/hid/Item.cc
        src/hid/Collection.cc
        src/hid/Control.cc
        src/Usages.cc
        src/hid/Usage.cc)

target_include_directories(hidx PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_link_libraries(hidx
        libusb-1.0
        Threads::Threads)

target_compile_features(hidx PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )

## 'make install' to the correct location
#install(TARGETS hidx EXPORT LibHidxConfig
#        ARCHIVE  DESTINATION lib
#        LIBRARY  DESTINATION lib
#        RUNTIME  DESTINATION bin)  # This is for Windows
#install(DIRECTORY include/ DESTINATION include)
#
## This makes the project importable from the install directory
## Put config file in per-project dir (name MUST match), can also
## just go into <prefix>/cmake.
#install(EXPORT LibHidxConfig NAMESPACE LibHidx DESTINATION share/libhidx/cmake)
#
## This makes the project importable from the build directory
#export(TARGETS hidx FILE LibHidxConfig.cmake)
