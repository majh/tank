cmake_minimum_required (VERSION 3.10)

project (water-tank)
add_executable(water-tank
    water-tank.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra -Wpointer-arith -Wredundant-decls -Weffc++ -Wextra -Wsign-conversion -pedantic -Wno-sign-compare -Wno-sign-conversion -Wno-variadic-macros -Wno-implicit-fallthrough -Wno-deprecated-declarations")
set(CMAKE_CC_FLAGS "-g -W -Wall -Wextra -Wpointer-arith")
