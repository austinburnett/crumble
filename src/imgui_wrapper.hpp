#pragma once

#include <GLFW/glfw3.h>

// OpenGL/GLFW specific ImGui wrapper.
// Handles the initial setup of ImGUI. It is not meant to replace all of ImGUI.
class ImguiWrapper {
public:
    ImguiWrapper(GLFWwindow* window);
    ~ImguiWrapper();
    ImguiWrapper(const ImguiWrapper&)            = delete;
    ImguiWrapper& operator=(const ImguiWrapper&) = delete;
    ImguiWrapper(ImguiWrapper&&)                 = delete;
    ImguiWrapper& operator=(ImguiWrapper&&)      = delete;

    void iteration();
    void render();
};
