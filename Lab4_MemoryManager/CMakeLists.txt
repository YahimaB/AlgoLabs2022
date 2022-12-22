cmake_minimum_required(VERSION 3.23)
project(Lab4_MemoryManager)
add_compile_definitions(_DEBUG)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab4_MemoryManager main.cpp FixedSizeAllocator.cpp CoalesceAllocator.cpp SystemAllocator.cpp SystemAllocator.h MemoryManager.cpp MemoryManager.h)
