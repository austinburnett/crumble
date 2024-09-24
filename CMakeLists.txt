cmake_minimum_required(VERSION 3.10)

project(falling-pixels)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Add the directory to the list of directories that the
# compiler uses to search for include files
include_directories(falling-pixels ../includes/ ../includes/glfw3/include/)

# Retrieve Dependent Libraries
find_library(COCOA_LIBRARY Cocoa)
find_library(OPENGL_LIBRARY OpenGL)
find_library(IO_LIBRARY IOKIT)
find_library(GLFW_LIBRARY glfw) 

# Create executable from source files called Learnopengl
#set(RUNTIME_OUTPUT_DIRECTORY ../)
add_executable(falling-pixels ../main.cpp ../glad.c ../square.cpp ../particle.cpp ../grid.cpp)

target_link_libraries(falling-pixels ${GLFW_LIBRARY})
target_link_libraries(falling-pixels ${COCOA_LIBRARY})
target_link_libraries(falling-pixels ${OPENGL_LIBRARY})
target_link_libraries(falling-pixels ${IO_LIBRARY})
