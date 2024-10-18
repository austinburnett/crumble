// Ensure GLFW doesn't include the OpenGL header.
#define GLFW_INCLUDE_NONE

// Autocomplete works better when using OpenGL headers.
#define DEVELOP true

#if DEVELOP
#include <OpenGL/OpenGL.h>
#else 
#include <glad/glad.h>
#endif

#include <iostream>
#include <thread>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hpp"
#include "includes/square.hpp"
#include "includes/particle.hpp"
#include "includes/grid.hpp"
#include "glfw_callbacks.cpp"

// Convert from the grid with the ranges [0, ROWS] and [0, COLUMNS] to ndc.
// Opengl expects vertices between [-1, 1] and a y-axis pointing up.
glm::vec3 grid_to_ndc(int i, int j, const int width, const int height);

// Displays a menu consisting of different particles types to render.
void display_particle_options_menu();

// Pixel Simulation Buffer
// [0][0] is the bottom-left corner of the window
Grid GRID;

static bool SHOULD_THREAD_RUN = true;

int main() {
    // Initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GlfW window and openGl context creation.
    GLFWwindow* window = glfwCreateWindow(ROWS, COLUMNS, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set glfw callbacks.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Load the OpenGL function pointers with glad.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO imgui_io = ImGui::GetIO();
    imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui Platform/Renderer backends.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Setup the graphics pipeline with the corresonding fragment and vertex shader.
    Shader ourShader("./resources/shader.vs", "./resources/shader.fs");
    Square square;

    // The render loop.
    while (!glfwWindowShouldClose(window)) {
        // Start the Dear ImGui frame. Calls to
        // ImGui must be made after ImGui::NewFrame().
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        display_particle_options_menu();

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        ourShader.use();
        glBindVertexArray(square.VAO);

        // Render all the particles to the framebuffer.
        for(int i = 0; i < ROWS; ++i) {
            for(int j = 0; j < COLUMNS; ++j) {
                if(GRID.at(i, j) != NULL && !GRID.at(i, j)->has_been_drawn) {
                    glm::mat4 model(1.0f);
                    glm::vec3 color = GRID.at(i, j)->get_color();
                    GRID.at(i, j)->has_been_drawn = true;
                    GRID.at(i, j)->update(i, j, GRID);

                    glm::vec3 translation = grid_to_ndc(i, j, ROWS, COLUMNS);
                    model = glm::translate(model, translation);
                    ourShader.setMat4("model", model);
                    ourShader.setVec3("particleColor", color);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }

        // Reset each particle's state so its only updated once per frame.
        reset_has_been_drawn_flags(GRID);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Cleanup
    SHOULD_THREAD_RUN = false;
    WORKER_THREAD.join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return (EXIT_SUCCESS);
}

// This plots particles in the grid corresponding to the cursor's location
// which are in screen coordinates [0, 0], is in the top-left whereas the
// position [0, 0] in the grid corresponds to the bottom-left corner.
void plot_particles_in_grid(double xpos, double ypos, GLFWwindow* window) {
    while(SHOULD_THREAD_RUN) {
        if(IS_THREAD_READY) {
            glfwGetCursorPos(window, &xpos, &ypos);
            if(int(xpos) >= 0 && int(COLUMNS-ypos) >= 0)
                if(int(xpos) < ROWS && int(COLUMNS-ypos) < COLUMNS)
                    // Flip the cursor's y-position such that it increases upwards.
                    GRID.at(int(xpos), int(COLUMNS-ypos)) = new WaterParticle();
        }
    }
}

glm::vec3 grid_to_ndc(int i, int j, const int width, const int height) {
    glm::vec3 point;
    point.x = (((float)i/width)*2)-1;
    point.y = 1.0*((((float)j/height)*2)-1);
    point.z = 0.0;
    return point;
}

void display_particle_options_menu() {
    ImGuiWindowFlags imgui_window_flags = 0;
    bool* p_open = NULL;
    imgui_window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::Begin("Particle Choices", p_open, imgui_window_flags);
    
    static int active_button = 0;
    ImGui::RadioButton("Water", &active_button, 0);
    ImGui::RadioButton("Sand", &active_button, 1);
    ImGui::RadioButton("Wall", &active_button, 2);

    ImGui::End();
}
