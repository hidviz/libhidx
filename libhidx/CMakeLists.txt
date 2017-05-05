cmake_minimum_required(VERSION 3.2)
project(libhidx VERSION 0.0.1 LANGUAGES C CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(hidx
        src/LibHidx.cc include/libhidx/LibHidx.hh
        src/Device.cc include/libhidx/Device.hh
        src/InterfaceHandle.cc include/libhidx/InterfaceHandle.hh
        src/Interface.cc include/libhidx/Interface.hh
        src/Parser.cc include/libhidx/Parser.hh
        src/hid/Item.cc include/libhidx/hid/Item.hh
        src/hid/Collection.cc include/libhidx/hid/Collection.hh
        src/hid/Control.cc include/libhidx/hid/Control.hh
        src/Usages.cc include/libhidx/Usages.hh
        src/hid/Usage.cc include/libhidx/hid/Usage.hh
        src/subprocess.hh
        src/Connector.cc src/Connector.hh)

target_include_directories(hidx PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_link_libraries(hidx
        Threads::Threads
        hidx_server
        )

target_compile_features(hidx PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidx PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)

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
