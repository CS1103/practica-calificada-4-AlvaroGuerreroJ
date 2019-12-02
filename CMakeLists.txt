# CMakeList.txt : CMake project for image_processing, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(pc4)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -Wextra -pthread")
endif()

add_executable (pc4 src/main.cpp src/lodepng.cpp src/util.cpp src/coordinate_generator.cpp)
