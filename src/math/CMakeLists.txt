set(ENGINE_MATH_PROJECT "Math")

file(
    GLOB_RECURSE ENGINE_MATH_CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/math/src/*.c*"
    "${CMAKE_SOURCE_DIR}/src/math/include/math/*.h*"
    "${CMAKE_SOURCE_DIR}/src/math/include/math/*.inl"
)

add_library(${ENGINE_MATH_PROJECT} SHARED ${ENGINE_MATH_CODE_LIST})

target_compile_features(${ENGINE_MATH_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${ENGINE_MATH_PROJECT} PRIVATE ${CMAKE_SOURCE_DIR}/src/math/include)

foreach(CODE_FILE IN ITEMS ${ENGINE_MATH_CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/math" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/math" "${ENGINE_EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "src" "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "include/math" "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()
