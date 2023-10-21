set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
cmake_minimum_required(VERSION 3.18)
project(Engines)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(OPENGL_PROJECT "OpenGL")

if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM ${CMAKE_VS_PLATFORM_NAME})
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/opengl/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM})
set(EXTERNAL_INCLUDE_DIR ${CMAKE_BINARY_DIR}/external/include)
set(EXTERNAL_SOURCE_DIR ${CMAKE_BINARY_DIR}/external/src)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_DEBUG_POSTFIX "" CACHE BOOL "" FORCE)
set(LIBRARY_SUFFIX "" CACHE BOOL "" FORCE)

#################################################################################################################################################
###   ASSIMP   ##################################################################################################################################
#################################################################################################################################################

file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/assimp-5.3.1.zip DESTINATION ${EXTERNAL_SOURCE_DIR}/)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL OFF FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_LIBRARY_SUFFIX "" CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXTERNAL_SOURCE_DIR}/assimp-5.3.1)

#################################################################################################################################################
###   GLFW   ####################################################################################################################################
#################################################################################################################################################

file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/glfw-3.3.8.zip DESTINATION ${EXTERNAL_SOURCE_DIR}/)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXTERNAL_SOURCE_DIR}/glfw-3.3.8)

#################################################################################################################################################
###   GLEW   ####################################################################################################################################
#################################################################################################################################################

file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/glew-2.2.0.zip DESTINATION ${EXTERNAL_SOURCE_DIR}/)
set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXTERNAL_SOURCE_DIR}/glew-2.2.0/build/cmake)
file(COPY ${EXTERNAL_SOURCE_DIR}/glew-2.2.0/include/GL DESTINATION ${EXTERNAL_INCLUDE_DIR}/)

#################################################################################################################################################
###   GLM   #####################################################################################################################################
#################################################################################################################################################

file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/glm-0.9.9.8.zip DESTINATION ${EXTERNAL_SOURCE_DIR}/)
file(COPY ${EXTERNAL_SOURCE_DIR}/glm/glm DESTINATION ${EXTERNAL_INCLUDE_DIR}/)

#################################################################################################################################################
###   STB   #####################################################################################################################################
#################################################################################################################################################

set(STB_FOLDER ${EXTERNAL_SOURCE_DIR}/stb)
file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/stb.zip DESTINATION ${EXTERNAL_SOURCE_DIR}/)
file(
    GLOB STB_CODE_LIST
    LIST_DIRECTORIES false
    "${STB_FOLDER}/*.h*"
)

foreach(STB_CODE_FILE IN ITEMS ${STB_CODE_LIST})
    file(COPY "${STB_CODE_FILE}" DESTINATION ${EXTERNAL_INCLUDE_DIR}/stb/)
endforeach()

#################################################################################################################################################
###   IMGUI   ###################################################################################################################################
#################################################################################################################################################

set(IMGUI_FOLDER ${EXTERNAL_SOURCE_DIR}/imgui-1.89.9)

file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/imgui-1.89.9.zip DESTINATION ${EXTERNAL_SOURCE_DIR}/)
file(
    GLOB IMGUI_CODE_LIST
    LIST_DIRECTORIES false
    "${IMGUI_FOLDER}/*.c*"
    "${IMGUI_FOLDER}/*.h*"
)

foreach(IMGUI_CODE_FILE IN ITEMS ${IMGUI_CODE_LIST})
    file(COPY "${IMGUI_CODE_FILE}" DESTINATION ${EXTERNAL_INCLUDE_DIR}/imgui/)
endforeach()

file(COPY ${IMGUI_FOLDER}/backends DESTINATION ${EXTERNAL_INCLUDE_DIR}/imgui/)

#################################################################################################################################################
#################################################################################################################################################
#################################################################################################################################################

include_directories(
    ${CMAKE_SOURCE_DIR}/src/opengl
    ${CMAKE_CURRENT_BINARY_DIR}/external/include
    ${CMAKE_CURRENT_BINARY_DIR}/external/include/stb
    ${CMAKE_CURRENT_BINARY_DIR}/external/include/imgui
)

# include(ExternalProject)
# ExternalProject_Add(assimplib
#     URL ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/assimp.zip
#     URL_HASH SHA256=5384877D53BE7B5BBF50C26AB3F054BEC91B3DF8614372DCD7240F44F61C509B
#     PREFIX ${CMAKE_CURRENT_BINARY_DIR}
#     BINARY_DIR external/build/assimp
#     SOURCE_DIR external/src/assimp
#     STAMP_DIR external/stamp/assimp
#     TMP_DIR external/tmp/assimp
#     CMAKE_ARGS -DBUILD_SHARED_LIBS:BOOL=OFF
#                -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT:BOOL=OFF
#                -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT:BOOL=OFF
#                -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
#                -DASSIMP_BUILD_TESTS:BOOL=OFF
#                -DASSIMP_BUILD_FBX_IMPORTER:BOOL=ON
#                -DASSIMP_BUILD_OBJ_IMPORTER:BOOL=ON
#                -DASSIMP_BUILD_ZLIB:BOOL=ON
#                -DASSIMP_LIBRARY_SUFFIX:STRING=
#                -DLIBRARY_SUFFIX:STRING=
#                -DCMAKE_DEBUG_POSTFIX:STRING=
#                -DASSIMP_INJECT_DEBUG_POSTFIX:BOOL=OFF
#                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_BINARY_DIR}/libs/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE}
#                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE}
#                -DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE}
#     INSTALL_COMMAND ""
# )

# ExternalProject_Add(freetypelib
#     URL ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/freetype.zip
#     URL_HASH SHA256=30F43A829F1D352054473B39E47D82E3135D828CF4DEF9C1EBD975F865AC4B73
#     PREFIX ${CMAKE_CURRENT_BINARY_DIR}
#     BINARY_DIR external/build/freetype
#     SOURCE_DIR external/src/freetype
#     STAMP_DIR external/stamp/freetype
#     TMP_DIR external/tmp/freetype
#     CMAKE_ARGS -DFT_DISABLE_ZLIB:BOOL=TRUE
#                -DFT_DISABLE_BZIP2:BOOL=TRUE
#                -DFT_DISABLE_PNG:BOOL=TRUE
#                -DFT_DISABLE_HARFBUZZ:BOOL=TRUE
#                -DFT_DISABLE_BROTLI:BOOL=TRUE
#                -DSKIP_INSTALL_ALL:BOOL=TRUE
#                -DDISABLE_FORCE_DEBUG_POSTFIX:BOOL=TRUE
#                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_BINARY_DIR}/libs/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE}
#                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE}
#                -DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE}
#     INSTALL_COMMAND ""
# )

# link_directories(${CMAKE_CURRENT_BINARY_DIR}/libs/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})

file(
    GLOB_RECURSE CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/opengl/*.c*"
    "${CMAKE_SOURCE_DIR}/src/opengl/*.h*"
    "${CMAKE_SOURCE_DIR}/src/opengl/*.glsl"
    "${EXTERNAL_INCLUDE_DIR}/imgui/imgui.cpp"
    "${EXTERNAL_INCLUDE_DIR}/imgui/imgui.h"
    "${EXTERNAL_INCLUDE_DIR}/imgui/imgui_draw.cpp"
    "${EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_glfw.h"
    "${EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
    "${EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3.h"
    "${EXTERNAL_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3_loader.h"
    "${EXTERNAL_INCLUDE_DIR}/imgui/imgui_internal.h"
    "${EXTERNAL_INCLUDE_DIR}/imgui/imgui_tables.cpp"
    "${EXTERNAL_INCLUDE_DIR}/imgui/imgui_widgets.cpp"
    "${EXTERNAL_INCLUDE_DIR}/stb/stb_image.h"
)

add_executable(${OPENGL_PROJECT} ${CODE_LIST})
# add_dependencies(${OPENGL_PROJECT} glfwlib)#assimplib glfwlib freetypelib)

target_compile_features(${OPENGL_PROJECT} PRIVATE cxx_std_17)

target_link_libraries(${OPENGL_PROJECT} assimp glfw glew_s)#glfw3 freetype)

if(UNIX)
    target_link_libraries(${OPENGL_PROJECT} X11)
elseif(MSVC)
    target_link_libraries(${OPENGL_PROJECT} opengl32)# "$<$<CONFIG:Debug,RelWithDebInfo>:glew32d>" "$<$<CONFIG:Release>:glew32>")
endif()

foreach(CODE_FILE IN ITEMS ${CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/opengl" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    # string(REPLACE "../thirdparty/include" "" GROUP_PATH "${GROUP_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/opengl" "${EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()

add_custom_command(TARGET ${OPENGL_PROJECT} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/thirdparty/resources $<TARGET_FILE_DIR:${OPENGL_PROJECT}>/resources)

add_custom_command(TARGET ${OPENGL_PROJECT} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/src/opengl/shaders $<TARGET_FILE_DIR:${OPENGL_PROJECT}>/shaders)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${OPENGL_PROJECT})
endif()
