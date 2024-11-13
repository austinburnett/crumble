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
#include "particle_sys/grid.hpp"
#include "particle_sys/particle_system.hpp"
#include "reuse/square.hpp"
#include "reuse/timer.hpp"

// Pixel Simulation Buffer
// [0][0] is the bottom-left corner of the window
Grid GRID;

int main() {
    ParticleSystem particle_system;
    Timer frame_timer;

    // Setup the graphics pipeline with the corresonding fragment and vertex shader.
    Shader ourShader("../resources/shader.vs", "../resources/shader.fs");
    Point point;

    // The render loop.
    while (!glfwWindowShouldClose(particle_system.window)) {
        // Start the Dear ImGui frame. Calls to
        // ImGui must be made after ImGui::NewFrame().
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        display_particle_options_menu(frame_timer.get_prev_elapsed_time().count());

        processInput(particle_system.window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        particle_system.draw(point.VAO, ourShader);

        glfwPollEvents();

        // Stop measuring the elapsed time between consecutive glfwSwapBuffers returns.
        frame_timer.stop();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(particle_system.window);

        // Start measuring the elapsed time between consecutive glfwSwapBuffers returns.
        frame_timer.start();
    }

    return (EXIT_SUCCESS);
}
