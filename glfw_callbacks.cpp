// This module contains implementations for any glfw callbacks that are set in main.cpp
// as well as any implementation for any related functions.

#include <iostream>
#include <thread>

#include <OpenGL/gl.h>
// Ensure GLFW doesn't include the OpenGL header
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Flag that dictates whether the worker thread will run
static bool IS_THREAD_READY = false;

static std::thread WORKER_THREAD;

// GLFW callback declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void error_callback(int error, const char* description);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Utility function declarations
void plot_particles_on_screen(double xpos, double ypos, GLFWwindow* window);

// Whenever the window size changes this callback function executes
inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process all input every frame
inline void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Whenever an error occurs this callback function executes
inline void error_callback(int error, const char* description) {
    std::cout << "Error: " << error << '\n' << description << '\n';
}

// Whenever a mouse button is pressed this callback function executes to
// append a new position at the location where the left mouse button was pressed
// so a primitive can be rendered at said position.
inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    int width, height;
    static bool has_started = 0;

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        IS_THREAD_READY = true;
        if(!has_started) {
            has_started = 1;
            WORKER_THREAD = std::thread(plot_particles_on_screen, xpos, ypos, window);
        }
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        IS_THREAD_READY = false;
    }
}
