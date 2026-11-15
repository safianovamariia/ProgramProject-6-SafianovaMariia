cmake_minimum_required(VERSION 3.10.0)
project(CPP_PP6 VERSION 0.1.0 LANGUAGES CXX)

if (MSVC)
    add_compile_options(/W3)
    add_compile_options(/std:c++20)
    add_compile_options(/EHsc)
else()
    add_compile_options(-Wall)
    add_compile_options(-std=c++20)
endif()

add_executable(CPP_PP6 main.cpp)
