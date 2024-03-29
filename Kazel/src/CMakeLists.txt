project(Kazel)

file(GLOB_RECURSE CORE_HEADER ./Core/*.h)
file(GLOB_RECURSE CORE_SOURCE ./Core/*.cpp)

file(GLOB_RECURSE EVENTS_HEADER ./Events/*.h)
file(GLOB_RECURSE EVENTS_SOURCE ./Events/*.cpp)

file(GLOB_RECURSE IMGUI_HEADER ./ImGui/*.h)
file(GLOB_RECURSE IMGUI_SOURCE ./ImGui/*.cpp)

file(GLOB_RECURSE MODELLOAD_HEADER ./ModelLoad/*.h)
file(GLOB_RECURSE MODELLOAD_SOURCE ./ModelLoad/*.cpp)

file(GLOB_RECURSE PLATFORM_HEADER ./Platform/*.h)
file(GLOB_RECURSE PLATFORM_SOURCE ./Platform/*.cpp)

file(GLOB_RECURSE RENDERER_HEADER ./Renderer/*.h)
file(GLOB_RECURSE RENDERER_SOURCE ./Renderer/*.cpp)

list(APPEND _sources ${CORE_SOURCE} ${CORE_HEADER}
    ${EVENTS_SOURCE} ${EVENTS_HEADER}
    ${IMGUI_HEADER} ${IMGUI_SOURCE}
    ${MODELLOAD_HEADER} ${MODELLOAD_SOURCE}
    ${PLATFORM_HEADER} ${PLATFORM_SOURCE}
    ${RENDERER_HEADER} ${RENDERER_SOURCE}
  )

add_library(${PROJECT_NAME} ${_sources})
add_library(Kazel::Kazel ALIAS ${PROJECT_NAME})
if(KZ_DEBUG)
  target_compile_definitions(${PROJECT_NAME} PRIVATE KZ_DEBUG)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE KZ_PLATFORM_WINDOW)

target_link_libraries(${PROJECT_NAME} PUBLIC glad::glad glfw glm::glm imgui spdlog::spdlog assimp::assimp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PUBLIC kzpch.h)