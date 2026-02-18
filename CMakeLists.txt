cmake_minimum_required(VERSION 3.24)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)
add_executable(MyProject 
src/main.cpp
src/window.cpp
src/input.cpp
src/event.cpp
src/timer.cpp
)
target_link_libraries(MyProject PRIVATE SDL3::SDL3)
