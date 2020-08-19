cmake_minimum_required(VERSION 3.0)

project(s_ptrlib VERSION 1.0.0)

#compile it as library with name s_ptr
add_library(s_ptrlib STATIC shared_pointer.cpp)