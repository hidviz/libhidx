cmake_minimum_required(VERSION 3.2)
project(libhidx_server VERSION 0.0.1 LANGUAGES C CXX)

add_executable(libhidx_server_daemon
        src/main.cc
        )

target_link_libraries(libhidx_server_daemon
        hidx_server
        )

target_compile_features(libhidx_server_daemon PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(libhidx_server_daemon PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)

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
