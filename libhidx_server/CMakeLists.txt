cmake_minimum_required(VERSION 3.2)
project(libhidx_server VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(libusb-1.0 REQUIRED)
add_library(libusb-1.0 UNKNOWN IMPORTED)
set_property(TARGET libusb-1.0 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBUSB_1_INCLUDE_DIRS})
set_property(TARGET libusb-1.0 PROPERTY IMPORTED_LOCATION ${LIBUSB_1_LIBRARY})

add_library(hidx_server
        src/Server.cc include/libhidx/server/Server.hh
        )

target_include_directories(hidx_server PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_link_libraries(hidx_server
        libusb-1.0
        hidx_buffer
        )

target_compile_features(hidx_server PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidx_server PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)

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
