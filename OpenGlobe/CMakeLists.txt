project(OpenGlobe)

FILE(GLOB_RECURSE SOURCE src/*.cpp)
FILE(GLOB_RECURSE HEADER src/*.h)
list(APPEND _source ${HEADER} ${SOURCE})

find_package(PythonInterp REQUIRED)

add_executable(${PROJECT_NAME} ${_source})

target_link_libraries(${PROJECT_NAME} PRIVATE Kazel::Kazel)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_custom_target(run ALL
    DEPENDS some_file)

add_custom_command(OUTPUT some_file
	COMMAND
		${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/copyResources.py ${PROJECT_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
VERBATIM
)
add_dependencies(OpenGlobe run)
