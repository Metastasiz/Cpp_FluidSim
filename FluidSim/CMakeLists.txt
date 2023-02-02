cmake_minimum_required(VERSION 3.17)
project(FluidSim)

set(CMAKE_CXX_STANDARD 14)

add_executable(FluidSim main.cpp BasicFrame.h MyConsole.h MyConsole.cpp Fluid.h Fluid.cpp Fluid.cpp)