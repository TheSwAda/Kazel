project(stdImage)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Kazel::stdImage ALIAS ${PROJECT_NAME})