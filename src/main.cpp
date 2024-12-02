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

#include "particle_system.hpp"
#include "glfw_wrapper.hpp"
#include "imgui_wrapper.hpp"
#include "gl_objects.hpp"
#include "timer.hpp"

// Pixel Simulation Buffer
// [0][0] is the bottom-left corner of the window
inline Grid GRID;

int main() {
    GlfwWrapper glfw(550, 550, "Crumble");
    glfw.set_callbacks();
    ImguiWrapper imgui(glfw.get_window());
    ParticleSystem particle_system(glfw.get_window());
    Timer frame_timer;

    // Setup the graphics pipeline with the corresonding fragment and vertex shader.
    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");
    Point point;

    // The render loop.
    while (!glfwWindowShouldClose(glfw.get_window())) {
        imgui.render_loop_iteration();

        //ImGui::ShowDemoWindow();
        display_particle_options_menu(frame_timer.get_prev_elapsed_time().count());

        particle_system.process_input(glfw.get_window());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        particle_system.draw(point.VAO, ourShader);

        glfw.poll_events();

        frame_timer.stop();
        imgui.render();
        glfw.swap_buffers();
        frame_timer.start();
    }

    return (EXIT_SUCCESS);
}
