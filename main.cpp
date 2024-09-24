// Ensure GLFW doesn't include the OpenGL header
#define GLFW_INCLUDE_NONE

// Autocomplete works better when using OpenGL headers
#define DEVELOP true

#if DEVELOP
#include <OpenGL/OpenGL.h>
#else 
#include <glad/glad.h>
#endif

#include <iostream>
#include <thread>

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hpp"
#include "glfw_callbacks.cpp"
#include "includes/square.hpp"
#include "includes/particle.hpp"
#include "includes/grid.hpp"

glm::vec3 screen_to_ndc(double x, double y, const int width, const int height);
glm::vec3 matrix_to_ndc(int i, int j, const int width, const int height);

// Settings
const unsigned int SCR_WIDTH = 550;
const unsigned int SCR_HEIGHT = 550;

// Pixel Simulation Buffer
// [0][0] is the bottom-left corner of the window
Grid grid;

// Deltatime
float DELTA_TIME = 0.0f; // Time between current and last frame 
float LAST_FRAME = 0.0f; // Time of last frame

static bool SHOULD_THREAD_RUN = true;

extern bool IS_THREAD_READY;
extern std::thread worker;

int main() {
    // Initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GlfW window and openGl context creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set glfw callbacks
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Load all OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("./resources/shader.vs", "./resources/shader.fs");
    Square square;

    double xpos, ypos;
    int width, height;
    glm::vec3 point;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        DELTA_TIME = currentFrame - LAST_FRAME;
        LAST_FRAME = currentFrame;

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        ourShader.use();
        glBindVertexArray(square.VAO);

        // Iterate over all pixels on the screen and draw a
        // particle there if the value in the grid is 1
        for(int i = 0; i < SCR_WIDTH; ++i) {
            for(int j = 0; j < SCR_HEIGHT; ++j) {
                if(grid.at(i, j) != NULL && !grid.at(i, j)->has_been_drawn) {
                    glm::mat4 model(1.0f);
                    glm::vec3 color = grid.at(i, j)->get_color();
                    grid.at(i, j)->has_been_drawn = true;
                    grid.at(i, j)->update(i, j, grid);

                    glm::vec3 translation = matrix_to_ndc(i, j, SCR_WIDTH, SCR_HEIGHT);
                    model = glm::translate(model, translation);
                    ourShader.setMat4("model", model);
                    ourShader.setVec3("particleColor", color);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }

        // Frame Upkeep
        reset_has_been_drawn_flags(grid);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    SHOULD_THREAD_RUN = false;
    worker.join();
    glfwDestroyWindow(window);
    glfwTerminate();
    return (EXIT_SUCCESS);
}

// Plots the particle on the screen at the cursor's location
void plot_particles_on_screen(double xpos, double ypos, GLFWwindow* window) {
    while(SHOULD_THREAD_RUN) {
        if(IS_THREAD_READY) {
            glfwGetCursorPos(window, &xpos, &ypos);
            if(int(xpos) >= 0 && int(SCR_HEIGHT-ypos) >= 0)
                if(int(xpos) < SCR_WIDTH && int(ypos) < SCR_HEIGHT)
                    grid.at(int(xpos), int(SCR_HEIGHT-ypos)) = new WaterParticle();
        }
    }
}

// Convert from screen coordinates to normalized device coordinates
// Screen coordinates are between [0, 1] and have an inverted y-axis
// Opengl expects vertices between [-1, 1] with a y-axis pointing up
glm::vec3 screen_to_ndc(double x, double y, const int width, const int height) {
    glm::vec3 point;
    point.x = ((x/width)*2)-1;
    point.y = -1.0*(((y/height)*2)-1);
    point.z = 0.0;
    return point;
}

// Convert from the matrix with the range [0, SCR_WIDTH] and [0, SCR_HEIGHT]
// corresponding to the rows and columns of the matrix to NDC where
// Opengl expects vertices between [-1, 1] with a y-axis pointing up
glm::vec3 matrix_to_ndc(int i, int j, const int width, const int height) {
    glm::vec3 point;
    point.x = (((float)i/width)*2)-1;
    point.y = 1.0*((((float)j/height)*2)-1);
    point.z = 0.0;
    return point;
}

