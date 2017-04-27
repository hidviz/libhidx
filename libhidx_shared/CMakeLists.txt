cmake_minimum_required(VERSION 3.2)
project(libhidx_shared LANGUAGES C CXX)

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS src/Buffer.proto)
add_library(hidx_shared ${PROTO_SRCS} ${PROTO_HDRS}
        src/Utils.cc include/libhidx/shared/Utils.hh
        include/libhidx/shared/Buffer.hh
        )
target_link_libraries(hidx_shared ${PROTOBUF_LIBRARIES} asio)

target_include_directories(hidx_shared PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_CURRENT_BINARY_DIR}
        PRIVATE
        src
        )

target_compile_features(hidx_shared PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidx_shared PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra> PUBLIC -DASIO_STANDALONE)

