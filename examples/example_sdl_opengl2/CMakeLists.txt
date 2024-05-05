set(CEF_TARGET "texocr")

add_executable(${CEF_TARGET}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../imgui_impl_sdl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../imgui_impl_sdl.h
    ${CMAKE_CURRENT_LIST_DIR}/../imgui_impl_opengl2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../imgui_impl_opengl2.h)


# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

# Determine the target output directory.
SET_CEF_TARGET_OUT_DIR()

#include the subdirectory containing our libs
include_directories(${_CEF_ROOT})

if(OS_WINDOWS)
  # Executable target.
#   add_executable(${CEF_TARGET} ${sources})
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  target_link_libraries(${CEF_TARGET} PRIVATE libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${CEF_TARGET} PRIVATE cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
  endif()

  # Add the custom manifest files to the executable.
  ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_LIST_DIR}" "${CEF_TARGET}" "exe")

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
endif()

set_property(TARGET ${CEF_TARGET} PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(${CEF_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../)

target_include_directories(${CEF_TARGET} PRIVATE ${CEF_ROOT}/include)

# target_link_options(${CEF_TARGET} PRIVATE /subsystem:windows)

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${CEF_TARGET} PRIVATE
                    SDL2::SDL2main
                    SDL2::SDL2-static
                    imgui
                    OpenGL::GL)

add_custom_command(TARGET ${CEF_TARGET}
  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_LIST_DIR}/mathrender.html
  $<TARGET_FILE_DIR:${CEF_TARGET}>/mathrender.html)

# Disable clip examples and tests
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")
set(CLIP_LIB_HEADER_FOLDER ${CMAKE_CURRENT_LIST_DIR}/clip CACHE STRING "Folder where to find clip library headers")

# In case that you have ${PNG_LIBRARY} set to support copy/paste images on Linux
#set(CLIP_X11_PNG_LIBRARY "${PNG_LIBRARY}")

# Add clip subdirectory to compile the library
add_subdirectory(clip)


target_link_libraries(${CEF_TARGET} PRIVATE clip)
target_include_directories(${CEF_TARGET} PRIVATE ${CLIP_LIB_HEADER_FOLDER})

find_package(cppzmq CONFIG REQUIRED)
target_link_libraries(${CEF_TARGET} PRIVATE cppzmq)
target_include_directories(${CEF_TARGET} PRIVATE ${Stb_INCLUDE_DIR})
