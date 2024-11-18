cmake_minimum_required(VERSION 3.10)

project(falling-pixels)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

#---------------------------------------------
#              Detect the Host OS
#---------------------------------------------
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    message(STATUS "Host system name: ${CMAKE_HOST_SYSTEM_NAME}")
endif()

#---------------------------------------------
#         Add Include Directories
#
# Include the directories to the list of paths 
# that the compiler uses to search for include 
# files. Include parent directory if you want
# includes that look like <GLFW/glfw3.h>
#---------------------------------------------
include_directories(falling-pixels ./src ./src/glfw3/include/ ./src/imgui/ ./src/imgui/backends/)

#---------------------------------------------
#              Find OpenGL
#---------------------------------------------
set(OPENGL_WINDOWS_PATH  "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64")
find_library(OPENGL_STATIC_LIBRARY NAMES OpenGL32 OpenGL32.Lib opengl32 OpenGL openGL32 HINTS ${OPENGL_WINDOWS_PATH})
find_file(OPENGL_DYNAMIC_LIBRARY opengl32.dll)

if (OPENGL_STATIC_LIBRARY)
    message(STATUS "Found the OpenGL static library: ${OPENGL_STATIC_LIBRARY}")
elseif(OPENGL_DYNAMIC_LIBRARY)
    message(STATUS "Found OpenGL windows dynamic library: ${OPENGL_DYNAMIC_LIBRARY}")
else()
    message(STATUS "Could not find the dynamic OpenGL library")
    message(STATUS "Could not find the static OpenGL library")
endif()

#---------------------------------------------
#              Find GLFW
#---------------------------------------------
find_library(GLFW_LIBRARY glfw) 

if (GLFW_LIBRARY)
    message(STATUS "Found the GLFW library: ${GLFW_LIBRARY}")
else()
    message(STATUS "Could not find the GLFW library")
endif()

#---------------------------------------------
#        Find MacOS Specific Libraries
#---------------------------------------------
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IO_LIBRARY IOKIT)
    if (COCOA_LIBRARY AND IO_LIBRARY)
        message(STATUS "Found the MacOS Specific Libraries")
    else()
        message(STATUS "Could not find the MacOS Specific Libraries")
    endif()
endif()

#---------------------------------------------
#         Create the Executable
#---------------------------------------------
add_executable(
falling-pixels
./src/main.cpp 
./src/glad/glad.c ./src/reuse/square.cpp ./src/particle_sys/grid.cpp ./src/particle_sys/particle.cpp ./src/glfw_callbacks.cpp ./src/particle_sys/particle_system.cpp
./src/reuse/timer.cpp
./src/imgui/imgui.cpp ./src/imgui/imgui_draw.cpp ./src/imgui/imgui_tables.cpp ./src/imgui/imgui_widgets.cpp ./src/imgui/imgui_demo.cpp
./src/imgui/backends/imgui_impl_opengl3.cpp ./src/imgui/backends/imgui_impl_glfw.cpp
)

#---------------------------------------------
#             Link the Libraries
#---------------------------------------------
target_link_libraries(falling-pixels ${GLFW_LIBRARY})
target_link_libraries(falling-pixels ${OPENGL_STATIC_LIBRARY})

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    target_link_libraries(falling-pixels ${COCOA_LIBRARY})
    target_link_libraries(falling-pixels ${IO_LIBRARY})
endif()
