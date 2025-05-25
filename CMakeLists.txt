cmake_minimum_required(VERSION 3.23)

set(VCPKG_LIBRARY_LINKAGE static CACHE STRING "Linkage of vcpkg libraries" FORCE)

project(applyMath)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(TBB CONFIG REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE TBB::tbb TBB::tbbmalloc)