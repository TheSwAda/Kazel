cmake_minimum_required(VERSION 3.20.0)
project(Kazel LANGUAGES CXX)
set(CMAKE_CONFIGURATION_TYPES  "Release;Debug")
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_TOOLCHAIN_FILE})

option(KZ_DEBUG "start kazel debug" ON)

add_subdirectory(Kazel)
add_subdirectory(Kazelnut)