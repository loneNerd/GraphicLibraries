set(ENGINE_CORE_PROJECT "Core")

find_package(OpenGL REQUIRED)

file(
    GLOB_RECURSE ENGINE_CORE_CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/core/src/*.c*"
    "${CMAKE_SOURCE_DIR}/src/core/include/core/*.h*"
    "${CMAKE_SOURCE_DIR}/src/core/include/core/*.inl"
)

add_library(${ENGINE_CORE_PROJECT} SHARED ${ENGINE_CORE_CODE_LIST})

target_compile_features(${ENGINE_CORE_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${ENGINE_CORE_PROJECT} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/math/include
    ${CMAKE_SOURCE_DIR}/src/opengl/include
    ${CMAKE_SOURCE_DIR}/src/tools/include
    ${ENGINE_EXTERNAL_INCLUDE_DIR}
)
target_link_libraries(${ENGINE_CORE_PROJECT} ${ENGINE_EXTERNAL_LIBS_DIR}/assimp-vc143-mt.lib ${ENGINE_EXTERNAL_LIBS_DIR}/glew32.lib OpenGL::GL Math OpenGL Tools)

foreach(CODE_FILE IN ITEMS ${ENGINE_CORE_CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/core" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/core" "${ENGINE_EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "src" "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "include/core" "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()
