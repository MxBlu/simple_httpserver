cmake_minimum_required(VERSION 3.6)
project(http_server)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c Socket.c Socket.h HTTPHandler.c HTTPHandler.h)
add_executable(http_server ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(http_server wsock32.lib)
    target_link_libraries(http_server Ws2_32.lib)
endif()