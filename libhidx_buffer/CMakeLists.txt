cmake_minimum_required(VERSION 3.2)
project(libhidx_buffer VERSION 0.0.1 LANGUAGES C CXX)



find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS buffer.proto)
add_library(hidx_buffer ${PROTO_SRCS} ${PROTO_HDRS} buffer_helper.cc buffer_helper.hh)
target_link_libraries(hidx_buffer ${PROTOBUF_LIBRARIES})

target_include_directories(hidx_buffer PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        #TODO INSTALL??
        )

target_compile_features(hidx_buffer PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidx_buffer PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)
