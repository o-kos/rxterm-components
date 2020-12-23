cmake_minimum_required(VERSION 3.17)
project(rxterm_components)

set(CMAKE_CXX_STANDARD 17)


set(RX_TERM "./../rxterm")
file(GLOB_RECURSE RX_TERM_INC ${RX_TERM}/include/rxterm/*.hpp)

include_directories(${RX_TERM}/include)

add_executable(rxterm_components
        ${RX_TERM_INC}
        ./components/phasediagram.hpp
        ./components/logview.hpp
        loren.h
        main.cpp)