cmake_minimum_required(VERSION 3.24)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Choose MSVC runtime library")

# set c++ standard
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(imbrowser)

add_library(imgui STATIC
    ${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/whereami.c
    ${CMAKE_CURRENT_LIST_DIR}/whereami.h)

target_sources(imgui PRIVATE ${CMAKE_CURRENT_LIST_DIR}/imgui_browser.cpp)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR})


set(CEF_ROOT ${CMAKE_CURRENT_LIST_DIR}/thirdparty/cef)

add_definitions(-DOS_WIN)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set_property(GLOBAL PROPERTY OS_FOLDERS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


find_package(CEF REQUIRED)
if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
endif()


# set(
#     CMAKE_RUNTIME_OUTPUT_DIRECTORY
#     ${CMAKE_HOME_DIRECTORY}/bin
#     )

# set(
#     CMAKE_LIBRARY_OUTPUT_DIRECTORY
#     ${CMAKE_HOME_DIRECTORY}/lib
#     )

# set(
#     TARGET_OUTPUT_DIRECTORY
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>
#     )

add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# Display configuration settings.
PRINT_CEF_CONFIG()

if(EXISTS "${CEF_ROOT}/tests")
  add_subdirectory(${CEF_ROOT}/tests/cefsimple)
  add_subdirectory(${CEF_ROOT}/tests/gtest)
  add_subdirectory(${CEF_ROOT}/tests/ceftests)
  add_subdirectory(${CEF_ROOT}/tests/cefclient)
endif()


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples/example_sdl_opengl2)
