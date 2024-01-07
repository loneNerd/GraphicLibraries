set(ENGINE_OPENGL_PROJECT "OpenGL")

find_package(OpenGL REQUIRED)

file(
    GLOB_RECURSE ENGINE_OPENGL_CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/opengl/*.c*"
    "${CMAKE_SOURCE_DIR}/src/opengl/*.h*"
    "${CMAKE_SOURCE_DIR}/src/opengl/*.inl"
    "${CMAKE_SOURCE_DIR}/src/opengl/*.glsl"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_draw.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_sdl2.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_sdl2.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3_loader.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_internal.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_tables.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_widgets.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/stb/stb_image.h"
)

add_executable(${ENGINE_OPENGL_PROJECT} ${ENGINE_OPENGL_CODE_LIST})

target_compile_features(${ENGINE_OPENGL_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${ENGINE_OPENGL_PROJECT} PRIVATE ${CMAKE_SOURCE_DIR}/src/opengl)
target_link_libraries(${ENGINE_OPENGL_PROJECT} ${ENGINE_LIBS_LIST} OpenGL::GL Logs Tools)

foreach(CODE_FILE IN ITEMS ${ENGINE_OPENGL_CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/opengl" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/opengl" "${ENGINE_EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()

add_custom_command(TARGET ${ENGINE_OPENGL_PROJECT} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/src/opengl/shaders $<TARGET_FILE_DIR:${ENGINE_OPENGL_PROJECT}>/opengl/shaders)
