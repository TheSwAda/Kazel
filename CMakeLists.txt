cmake_minimum_required(VERSION 3.20.0)


project(Kazel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CONFIGURATION_TYPES  "Release;Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
option(KZ_DEBUG "start kazel debug" ON)

add_compile_definitions(NOMINMAX)
add_subdirectory(Kazel)
add_subdirectory(OpenGlobe)
