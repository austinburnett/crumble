// Ensure GLFW doesn't include the OpenGL header
#define GLFW_INCLUDE_NONE

// Autocomplete works better when using OpenGL headers
#define DEVELOP true

#if DEVELOP
#include <OpenGL/OpenGL.h>
#else 
#include <glad/glad.h>
#endif

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hpp"
#include "includes/square.hpp"
#include "includes/particle.hpp"

void plot_particles_on_screen(double xpos, double ypos, GLFWwindow* window);
glm::vec3 screen_to_ndc(double x, double y, const int width, const int height);
glm::vec3 matrix_to_ndc(int i, int j, const int width, const int height);
bool is_particle_in_row(int row);
void update_particles_on_screen(int start, int end, Shader& ourShader);

// GLFW callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void error_callback(int error, const char* description);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Settings
const unsigned int SCR_WIDTH = 550;
const unsigned int SCR_HEIGHT = 550;

// Deltatime
float DELTA_TIME = 0.0f; // Time between current and last frame 
float LAST_FRAME = 0.0f; // Time of last frame

// Flag that dictates whether or not the worker thread will plot squares
static bool READY = false;

// Flag that terminates the worker thread's while loop
static bool IS_RUNNING = true;

static std::thread worker;

// Pixel Simulation Buffer
// [0][0] is the bottom-left corner of the window
Particle* grid[SCR_WIDTH][SCR_HEIGHT];

int main() {
    // Initialize and configure glfw
    glfwSetErrorCallback(error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Glfw window and openGl context creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set glfw callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Load all OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Initialize all grid values to NULL which indicates no particles are on the screen
    for(int i = 0; i < SCR_WIDTH; ++i) {
        for(int j = 0; j < SCR_HEIGHT; ++j) {
            grid[i][j] = NULL;
        }
    }

    Shader ourShader("./resources/shader.vs", "./resources/shader.fs");
    Square square;

    double xpos, ypos;
    int width, height;
    glm::vec3 point;

    std::thread render_worker;

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
            if(!is_particle_in_row(i))
                continue;
            for(int j = 0; j < SCR_HEIGHT; ++j) {
                if(grid[i][j] != NULL && !grid[i][j]->has_been_drawn) {
                    glm::mat4 model(1.0f);
                    glm::vec3 color = grid[i][j]->get_color();

                    if(j >= 0 && i >= 0) {
                        grid[i][j]->has_been_drawn = true;
                        grid[i][j]->update(i, j, grid);
                    }
                    glm::vec3 translation = matrix_to_ndc(i, j, SCR_WIDTH, SCR_HEIGHT);
                    model = glm::translate(model, translation);
                    ourShader.setMat4("model", model);
                    ourShader.setVec3("particleColor", color);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }
        // Reset particle's has_been_drawn flag
        for(int i = 0; i < SCR_WIDTH; ++i) {
            for(int j = 0; j < SCR_HEIGHT; ++j) {
                if(grid[i][j] != NULL)
                    grid[i][j]->has_been_drawn = false;
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    IS_RUNNING = false;
    worker.join();
    glfwDestroyWindow(window);
    glfwTerminate();
    return (EXIT_SUCCESS);
}

// Process all input every frame
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Whenever the window size changes this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Whenever an error occurs this callback function executes
void error_callback(int error, const char* description) {
    std::cout << "Error: " << error << '\n' << description << '\n';
}

// Plots the particle on the screen at the cursor's location
void plot_particles_on_screen(double xpos, double ypos, GLFWwindow* window) {
    while(IS_RUNNING) {
        if(READY) {
            glfwGetCursorPos(window, &xpos, &ypos);
            if(int(xpos) >= 0 && int(SCR_HEIGHT-ypos) >= 0)
                if(int(xpos) < SCR_WIDTH && int(ypos) < SCR_HEIGHT)
                    grid[int(xpos)][int(SCR_HEIGHT-ypos)] = new WaterParticle();
        }
    }
}

// Whenever a mouse button is pressed this callback function executes to
// append a new position at the location where the left mouse button was pressed
// so a primitive can be rendered at said position.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    int width, height;
    static bool has_started = 0;

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        READY = true;
        if(!has_started) {
            has_started = 1;
            worker = std::thread(plot_particles_on_screen, xpos, ypos, window);
        }
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        READY = false;
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

// Determines whether or not a particle is present in a row
bool is_particle_in_row(int row) {
    for(int i = 0; i < SCR_WIDTH; ++i) {
        if(grid[row][i] != NULL) {
            return true;
        }
    }
    return false;
}

// Updates all of the particles within the given start and end range
void update_particles_on_screen(int start, int end, Shader& ourShader) {
        for(int i = start; i < end; ++i) {
            if(!is_particle_in_row(i))
                continue;
            for(int j = 0; j < SCR_HEIGHT; ++j) {
                glm::mat4 model(1.0f);
                if(grid[i][j] != NULL) {
                    glm::vec3 color;
                    color = grid[i][j]->get_color();

                    if((j > 0 && i > 0) && (i < SCR_WIDTH && j < SCR_HEIGHT)) {
                        grid[i][j]->update(i, j, grid);
                    }
                    glm::vec3 translation = matrix_to_ndc(i, j, SCR_WIDTH, SCR_HEIGHT);
                    model = glm::translate(model, translation);
                    ourShader.setMat4("model", model);
                    ourShader.setVec3("particleColor", color);
                }
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
}
