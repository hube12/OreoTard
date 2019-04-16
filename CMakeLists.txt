cmake_minimum_required(VERSION 3.13)
project(OreoTard)

set(CMAKE_CXX_STANDARD 17)

add_executable(OreoTard generationByCubitect/javarnd.h generationByCubitect/generator.cpp generationByCubitect/layers.cpp entrypoint.cc)