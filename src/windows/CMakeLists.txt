set(ENGINE_WINDOWS_PROJECT "Windows")

include_directories(${ENGINE_EXTERNAL_INCLUDE_DIR}/windows)

file(
    GLOB_RECURSE ENGINE_WINDOWS_CODE_LIST
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/windows/src/*.c*"
    "${CMAKE_SOURCE_DIR}/src/windows/include/windows/*.h*"
    "${CMAKE_SOURCE_DIR}/src/windows/include/windows/*.inl"
)

add_library(${ENGINE_WINDOWS_PROJECT} SHARED ${ENGINE_WINDOWS_CODE_LIST})

target_compile_features(${ENGINE_WINDOWS_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${ENGINE_WINDOWS_PROJECT} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/logs/include
    ${CMAKE_SOURCE_DIR}/src/windows/include
    ${ENGINE_EXTERNAL_INCLUDE_DIR}
)
target_link_libraries(${ENGINE_WINDOWS_PROJECT} ${ENGINE_EXTERNAL_LIBS_DIR}/SDL2.lib Logs)

foreach(CODE_FILE IN ITEMS ${ENGINE_WINDOWS_CODE_LIST})
    get_filename_component(CODE_FILE_PATH "${CODE_FILE}" PATH)
    file(RELATIVE_PATH CODE_FILE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/windows" "${CODE_FILE_PATH}")
    string(REPLACE "\\" "/" GROUP_PATH "${CODE_FILE_RELATIVE_PATH}")
    file(RELATIVE_PATH EXTERNAL_INCLUDE_RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src/windows" "${ENGINE_EXTERNAL_INCLUDE_DIR}/")
    string(REPLACE ${EXTERNAL_INCLUDE_RELATIVE_PATH} "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "src" "" GROUP_PATH "${GROUP_PATH}")
    string(REPLACE "include/windows" "" GROUP_PATH "${GROUP_PATH}")
    source_group("${GROUP_PATH}" FILES "${CODE_FILE}")
endforeach()
