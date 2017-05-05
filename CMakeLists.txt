cmake_minimum_required(VERSION 3.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
find_package(asio REQUIRED)

add_subdirectory(libhidx)
add_subdirectory(libhidx_server)
add_subdirectory(libhidx_server_daemon)
