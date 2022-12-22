cmake_minimum_required(VERSION 3.23)
project(Lab4_MemoryManager)
add_compile_definitions(_DEBUG)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab4_MemoryManager main.cpp FixedSizeAllocator.cpp CoalesceAllocator.cpp)
