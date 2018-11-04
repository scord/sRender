cmake_minimum_required (VERSION 2.8.11)
project (sRender)

set(SOURCES src/core/smath.cpp
            src/main.cpp
            src/core/camera.cpp
            src/core/sampler.cpp
            src/samplers/pixelsampler.cpp
            src/core/geometry.cpp
            src/core/integrator.cpp
            src/core/brdf.cpp
            src/brdfs/diffuse.cpp
            src/brdfs/mirror.cpp
            src/core/scene.cpp
            src/core/kdtree.cpp
   )

set (CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")  

add_executable(sRender ${SOURCES})

