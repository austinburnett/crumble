#include <iostream>
#include <thread>

#include <OpenGL/gl.h>
// Ensure GLFW doesn't include the OpenGL header
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Flag that dictates whether the worker thread will run
inline bool IS_THREAD_READY = false;

inline std::thread WORKER_THREAD;

// GLFW callback declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void error_callback(int error, const char* description);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Utility function declarations
void plot_particles_in_grid(GLFWwindow* window);

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
// start or stop the particle-plotting thread depending on the mouse button
// that was pressed.
inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    int width, height;
    static bool has_started = 0;

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        IS_THREAD_READY = true;
        if(!has_started) {
            has_started = 1;
            WORKER_THREAD = std::thread(plot_particles_in_grid, window);
        }
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        IS_THREAD_READY = false;
    }
}
