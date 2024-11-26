// Ensure GLFW doesn't include the OpenGL header.
#define GLFW_INCLUDE_NONE

// Autocomplete works better when using OpenGL headers.
#define DEVELOP true

#if DEVELOP
#include <OpenGL/OpenGL.h>
#else 
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "reuse/shader.hpp"
#include "grid.hpp"
#include "particle_system.hpp"
#include "glfw_wrapper.hpp"
#include "imgui_wrapper.hpp"
#include "reuse/square.hpp"
#include "reuse/timer.hpp"

// Pixel Simulation Buffer
// [0][0] is the bottom-left corner of the window
Grid GRID;

int main() {
    GlfwWrapper glfw(550, 550, "Crumble");
    ImguiWrapper imgui(glfw.get_window());
    ParticleSystem particle_system;
    Timer frame_timer;

    // Setup the graphics pipeline with the corresonding fragment and vertex shader.
    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");
    Point point;

    // The render loop.
    while (!glfwWindowShouldClose(glfw.get_window())) {
        imgui.iteration();

        //ImGui::ShowDemoWindow();
        display_particle_options_menu(frame_timer.get_prev_elapsed_time().count());

        processInput(glfw.get_window());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        particle_system.draw(point.VAO, ourShader);

        glfwPollEvents();

        // Stop measuring the elapsed time between consecutive glfwSwapBuffers returns.
        frame_timer.stop();

        imgui.render();
        glfwSwapBuffers(glfw.get_window());

        // Start measuring the elapsed time between consecutive glfwSwapBuffers returns.
        frame_timer.start();
    }

    return (EXIT_SUCCESS);
}
