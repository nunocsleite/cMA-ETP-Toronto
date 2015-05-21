
set(CMAKE_CXX_FLAGS "-fPIC")

# In order to use C++ 11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

cmake_minimum_required(VERSION 2.8)

# For Release compilation
set(CMAKE_BUILD_TYPE Release)


# About this project
#
# SO-Toronto - Single Objective Optimisation for Toronto benchmarks
project(SO-Toronto)


include_directories(${SO_SOURCE_DIR}/../ext/include/ParadisEO-2.0/eo/src/)
include_directories(${SO_SOURCE_DIR}/../ext/include/ParadisEO-2.0/mo/src/)
include_directories(${SO_SOURCE_DIR}/../ext/include/ParadisEO-2.0/moeo/src/)


include_directories(exes)
include_directories(lib) 
include_directories(lib/chromosome) 

#
# Add subfolders
#
add_subdirectory(lib)
add_subdirectory(exes)
