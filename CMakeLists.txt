set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
cmake_minimum_required(VERSION 3.18)

OPTION(ENGINE_OPT_BUILD_DEPENDENCIES
  "Set to ON to build external dependencies"
  OFF
)

project(Engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM ${CMAKE_VS_PLATFORM_NAME})
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(ENGINE_EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/include)
set(ENGINE_EXTERNAL_LIBS_DIR ${CMAKE_SOURCE_DIR}/thirdparty/libs)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(CMAKE_DEBUG_POSTFIX "" CACHE BOOL "" FORCE)
set(LIBRARY_SUFFIX "" CACHE BOOL "" FORCE)

if(ENGINE_OPT_BUILD_DEPENDENCIES)
    execute_process(COMMAND powershell -ExecutionPolicy Bypass -File ${CMAKE_SOURCE_DIR}/thirdparty/external_libs/build_dependencies.ps1)
endif()

file(
    GLOB_RECURSE ENGINE_BINS_LIST
    "${CMAKE_SOURCE_DIR}/thirdparty/bins/*.*"
)

file(
    GLOB ENGINE_ASSETS_LIST
    "${CMAKE_SOURCE_DIR}/thirdparty/assets/*"
)

# include_directories(
#     ${CMAKE_SOURCE_DIR}/src/core/include
#     ${CMAKE_SOURCE_DIR}/src/engine/include
#     ${CMAKE_SOURCE_DIR}/src/logs/include
#     ${CMAKE_SOURCE_DIR}/src/math/include
#     ${CMAKE_SOURCE_DIR}/src/opengl/include
#     ${CMAKE_SOURCE_DIR}/src/tools/include
#     ${CMAKE_SOURCE_DIR}/src/windows/include
#     ${ENGINE_EXTERNAL_INCLUDE_DIR}
#     ${ENGINE_EXTERNAL_INCLUDE_DIR}/imgui
#     ${ENGINE_EXTERNAL_INCLUDE_DIR}/stb
# )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM})

foreach (BIN_FILE ${ENGINE_BINS_LIST})
    file(COPY ${BIN_FILE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach ()

foreach (ASSET_FILE ${ENGINE_ASSETS_LIST})
    file(COPY ${ASSET_FILE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
endforeach ()

# add_subdirectory(${CMAKE_SOURCE_DIR}/src/vulkan)
# if(WIN32)
#     add_subdirectory(${CMAKE_SOURCE_DIR}/src/directx11)
#     add_subdirectory(${CMAKE_SOURCE_DIR}/src/directx12)
# endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/src/core)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/engine)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/logs)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/math)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/opengl)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/tools)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/ui)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/windows)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine)
endif()
