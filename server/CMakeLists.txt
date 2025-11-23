cmake_minimum_required(VERSION 3.16)
project(prakt5_server LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Network)
add_executable(server
    src/main.cpp
    src/udpserver.cpp
    src/udpserver.h
    src/complex.h
    src/array.h
    src/polynom.h
)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(server PRIVATE Qt6::Core Qt6::Network)
