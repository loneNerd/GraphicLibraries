set(ENGINE_OPENGL_PROJECT "OpenGL")

find_package(OpenGL REQUIRED)

file(
    GLOB_RECURSE ENGINE_OPENGL_CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/opengl/src/*.c*"
    "${CMAKE_SOURCE_DIR}/src/opengl/include/opengl/*.h*"
    "${CMAKE_SOURCE_DIR}/src/opengl/include/opengl/*.inl"
    "${CMAKE_SOURCE_DIR}/src/opengl/shaders/*.glsl"
)

add_library(${ENGINE_OPENGL_PROJECT} SHARED ${ENGINE_OPENGL_CODE_LIST})

target_compile_features(${ENGINE_OPENGL_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${ENGINE_OPENGL_PROJECT} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/logs/include
    ${CMAKE_SOURCE_DIR}/src/math/include    
    ${CMAKE_SOURCE_DIR}/src/opengl/include
    ${CMAKE_SOURCE_DIR}/src/tools/include
    ${ENGINE_EXTERNAL_INCLUDE_DIR}
)
target_link_libraries(${ENGINE_OPENGL_PROJECT} ${ENGINE_EXTERNAL_LIBS_DIR}/glew32.lib OpenGL::GL Logs Math Tools)

foreach(CODE_FILE IN ITEMS ${ENGINE_OPENGL_CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/opengl" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/opengl" "${ENGINE_EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "src" "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "include/opengl" "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()
