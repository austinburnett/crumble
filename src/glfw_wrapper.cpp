#include <csignal>
#include <iostream>

#include <glad/glad.h>

#include "glfw_wrapper.hpp"
#include "glfw_callbacks.cpp"

extern std::thread WORKER_THREAD;

GlfwWrapper::GlfwWrapper(const int width, const int height, const char* title) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // GlfW window and openGl context creation.
        m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (m_window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            raise(-1);
        }
        glfwMakeContextCurrent(m_window);

        // Set glfw callbacks.
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetErrorCallback(error_callback);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetMouseButtonCallback(m_window, mouse_button_callback);

        // Load the OpenGL function pointers with glad.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            raise(-1);
        }
}

GlfwWrapper::~GlfwWrapper() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
    WORKER_THREAD.join();
}

GLFWwindow* GlfwWrapper::get_window() {
    return m_window;
}
