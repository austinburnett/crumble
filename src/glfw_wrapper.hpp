#pragma once

#include <GLFW/glfw3.h>


// Handles the initial setup of GLFW.
// It is not meant to replace all of GLFW.
class GlfwWrapper {
public:
    GlfwWrapper(const int width, const int height, const char* title);
    ~GlfwWrapper();
    GlfwWrapper(const GlfwWrapper&)            = delete;
    GlfwWrapper& operator=(const GlfwWrapper&) = delete;
    GlfwWrapper(GlfwWrapper&&)                 = delete;
    GlfwWrapper& operator=(GlfwWrapper&&)      = delete;

    GLFWwindow* get_window();
    void set_callbacks();

private:
    GLFWwindow* m_window = NULL;
};
