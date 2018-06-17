cmake_minimum_required(VERSION 3.10)
project(TecnServer)

set(CMAKE_CXX_STANDARD 11)

include_directories(ChSocket)

find_package(Threads)

add_compile_options("-L/lib -ljson-c")
link_libraries("-L/lib -ljson-c")

add_executable(TecnoServer
        ChSocket/Socket.c
        ChSocket/Socket.h
        ChSocket/Socket_Cliente.c
        ChSocket/Socket_Cliente.h
        ChSocket/Socket_Servidor.c
        ChSocket/Socket_Servidor.h
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        cmake-build-debug/Server
        cmake-build-debug/Server.cbp
        CMakeLists.txt
        main.c constants.c constants.h
        main.c ChSocket/Commands.h ChSocket/Commands.c)

target_link_libraries(TecnoServer ${CMAKE_THREAD_LIBS_INIT})
