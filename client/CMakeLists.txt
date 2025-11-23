cmake_minimum_required(VERSION 3.16)
project(prakt5_client LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Core)
add_executable(client
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/udpclient.cpp
    src/udpclient.h
    src/complex.h
    src/array.h
    src/polynom.h
)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(client PRIVATE Qt6::Widgets Qt6::Network Qt6::Core)
