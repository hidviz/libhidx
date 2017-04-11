cmake_minimum_required(VERSION 3.2)
project(libhidx_helper VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(libusb-1.0 REQUIRED)
add_library(libusb-1.0 SHARED IMPORTED)
set_property(TARGET libusb-1.0 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBUSB_1_INCLUDE_DIRS})
set_property(TARGET libusb-1.0 PROPERTY IMPORTED_LOCATION ${LIBUSB_1_LIBRARY})

add_executable(libhidx_helper main.cc)

target_link_libraries(libhidx_helper
        libusb-1.0
        hidx_buffer
        )

target_compile_features(libhidx_helper PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(libhidx_helper PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)
