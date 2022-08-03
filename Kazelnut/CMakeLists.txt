project(Kazelnut)

FILE(GLOB_RECURSE SOURCE src/*.cpp)
FILE(GLOB_RECURSE HEADER src/*.h)

list(APPEND _source ${HEADER} ${SOURCE})
add_executable(${PROJECT_NAME} ${_source})
target_link_libraries(${PROJECT_NAME} Kazel::Kazel)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})