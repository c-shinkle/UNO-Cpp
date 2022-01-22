cmake_minimum_required(VERSION 3.5)
 
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project(UNO-Cpp)

add_subdirectory(unoLib)
add_subdirectory(unoApp)
