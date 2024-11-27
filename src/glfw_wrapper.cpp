#include <csignal>
#include <iostream>

#include <glad/glad.h>

#include "glfw_wrapper.hpp"

extern std::thread G_WORKER_THREAD;
extern bool        G_DO_WORK_IN_THREAD;
extern bool        G_KEEP_THREAD_RUNNING;

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
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // Load the OpenGL function pointers with glad.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            raise(-1);
        }
}

GlfwWrapper::~GlfwWrapper() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

GLFWwindow* GlfwWrapper::get_window() {
    return m_window;
}

void GlfwWrapper::set_callbacks() {
    glfwSetErrorCallback([](int error, const char* description) {
        std::cout << "Error: " << error << '\n' << description << '\n';
    });
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            G_DO_WORK_IN_THREAD = true;
        }
        else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            G_DO_WORK_IN_THREAD = false;
        }
    });
}
