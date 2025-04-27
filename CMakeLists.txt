cmake_minimum_required(VERSION 3.16)
project(MySDL3Project)

set(CMAKE_CXX_STANDARD 17)

# Find SDL3
set(CMAKE_PREFIX_PATH "C:/SDL3" ${CMAKE_PREFIX_PATH})
find_package(SDL3 REQUIRED)

add_executable(MySDL3Project main.cpp)

# Link SDL3
target_link_libraries(MySDL3Project PRIVATE SDL3::SDL3)
target_include_directories(MySDL3Project PRIVATE "C:/SDL3/x86_64-w64-mingw32/include/SDL3")
