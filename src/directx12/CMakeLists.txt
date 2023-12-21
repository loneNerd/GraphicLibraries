set(ENGINE_DIRECTX12_PROJECT "DirectX12")

file(
    GLOB_RECURSE ENGINE_DIRECTX12_CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/directx12/*.c*"
    "${CMAKE_SOURCE_DIR}/src/directx12/*.h*"
    "${CMAKE_SOURCE_DIR}/src/directx12/*.inl"
    "${CMAKE_SOURCE_DIR}/src/directx12/*.glsl"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_draw.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_win32.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_win32.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_dx12.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_dx12.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_internal.h"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_tables.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui/imgui_widgets.cpp"
    "${ENGINE_EXTERNAL_INCLUDE_DIR}/stb/stb_image.h"
)

add_executable(${ENGINE_DIRECTX12_PROJECT} ${ENGINE_DIRECTX12_CODE_LIST})

target_compile_features(${ENGINE_DIRECTX12_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${ENGINE_DIRECTX12_PROJECT} PRIVATE ${CMAKE_SOURCE_DIR}/src/directx12)
# target_include_directories(${ENGINE_DIRECTX12_PROJECT} PRIVATE ${Vulkan_INCLUDE_DIRS})
# target_link_libraries(${ENGINE_DIRECTX12_PROJECT} ${ENGINE_LIBS_LIST} Vulkan::Vulkan)
target_link_libraries(${ENGINE_DIRECTX12_PROJECT} ${ENGINE_LIBS_LIST} d3d12.lib dxgi.lib D3DCompiler.lib)

foreach(CODE_FILE IN ITEMS ${ENGINE_DIRECTX12_CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/directx12" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/directx12" "${ENGINE_EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()

add_custom_command(TARGET ${ENGINE_DIRECTX12_PROJECT} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/src/directx12/shaders/ $<TARGET_FILE_DIR:${ENGINE_DIRECTX12_PROJECT}>/directx12/shaders)
