set(ENGINE_VULKAN_PROJECT "Vulkan")

find_package(Vulkan REQUIRED)

include_directories(${ENGINE_EXTERNAL_INCLUDE_DIR}/SDL2)

file(
    GLOB_RECURSE ENGINE_VULKAN_CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/vulkan/*.c*"
    "${CMAKE_SOURCE_DIR}/src/vulkan/*.h*"
    "${CMAKE_SOURCE_DIR}/src/vulkan/*.inl"
    "${CMAKE_SOURCE_DIR}/src/vulkan/*.glsl"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_draw.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_sdl2.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_sdl2.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_vulkan.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_vulkan.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_internal.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_tables.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_widgets.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/stb/stb_image.h"
)

add_executable(${ENGINE_VULKAN_PROJECT} ${ENGINE_VULKAN_CODE_LIST})

target_compile_features(${ENGINE_VULKAN_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${ENGINE_VULKAN_PROJECT} PRIVATE ${CMAKE_SOURCE_DIR}/src/vulkan)
target_include_directories(${ENGINE_VULKAN_PROJECT} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${ENGINE_VULKAN_PROJECT} ${ENGINE_EXTERNAL_LIBS_DIR}/SDL2.lib Vulkan::Vulkan)

foreach(CODE_FILE IN ITEMS ${ENGINE_VULKAN_CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/vulkan" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/vulkan" "${ENGINE_EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()

add_custom_command(TARGET ${ENGINE_VULKAN_PROJECT} POST_BUILD
                    COMMAND cmd /c ${CMAKE_SOURCE_DIR}/src/vulkan/shaders/compile.bat)

add_custom_command(TARGET ${ENGINE_VULKAN_PROJECT} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/src/vulkan/shaders/ $<TARGET_FILE_DIR:${ENGINE_VULKAN_PROJECT}>/vulkan/shaders)
