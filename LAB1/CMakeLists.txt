cmake_minimum_required(VERSION 3.10)
project(NetworkTransfer C)

set(CMAKE_C_STANDARD 99)

if(WIN32)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

set(SOURCES
    server-tcp.c
    client-tcp.c
    server-udp.c
    client-udp.c
)

add_executable(server-tcp server-tcp.c)
if(WIN32)
    target_link_libraries(server-tcp ws2_32)
endif()

add_executable(client-tcp client-tcp.c)
if(WIN32)
    target_link_libraries(client-tcp ws2_32)
endif()

add_executable(server-udp server-udp.c)
if(WIN32)
    target_link_libraries(server-udp ws2_32)
endif()

add_executable(client-udp client-udp.c)
if(WIN32)
    target_link_libraries(client-udp ws2_32)
endif()
