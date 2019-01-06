cmake_minimum_required (VERSION 2.8.11)
project (sRender)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")  

add_executable(sRender ${SOURCES})

