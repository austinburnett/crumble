// Ensure GLFW doesn't include the OpenGL header
#include "glm/ext/matrix_transform.hpp"
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
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hpp"
#include "includes/square.hpp"

glm::vec3 screen_to_ndc(double x, double y, const int width, const int height);
glm::vec3 matrix_to_ndc(int i, int j, const int width, const int height);

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

// The list of positions needs to be globally accessible
static std::vector<glm::vec3> square_translations = {};

// Pixel Simulation Buffer
//int pixel_buffer[100][100];
int grid[SCR_WIDTH][SCR_HEIGHT];

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

    // Underlying grid data structure of the pixel simulation
    //int grid[SCR_WIDTH][SCR_HEIGHT];

    for(int i = 0; i < SCR_WIDTH; ++i) {
        for(int j = 0; j < SCR_HEIGHT; ++j) {
            grid[i][j] = 0;
        }
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        ourShader.use();
        glBindVertexArray(square.VAO);

        // Iterate over all pixels on the screen and draw a
        // particle there if the value in the grid is 1
        for(int i = 0; i < SCR_WIDTH; ++i) {
            for(int j = 0; j < SCR_HEIGHT; ++j) {
                glm::mat4 model(1.0f);
                if(grid[i][j] == 1) {
                    if(j > 0 && i > 0 && i < SCR_WIDTH && j < SCR_HEIGHT) {
                        // Move particle down one block if nothing is there
                        if(grid[i][j-1] == 0) {
                            grid[i][j] = 0;
                            grid[i][j-1] = 1;
                        }
                        // Move particle left by one block if nothing is there
                        else if(grid[i-1][j-1] == 0) {
                            grid[i][j] = 0;
                            grid[i-1][j-1] = 1;
                        }
                        // Move particle right by one block if nothing is there
                        else if(grid[i+1][j-1] == 0) {
                            grid[i][j] = 0;
                            grid[i+1][j-1] = 1;
                        }
                        glm::vec3 translation = matrix_to_ndc(i, j, SCR_WIDTH, SCR_HEIGHT);
                        model = glm::translate(model, translation);
                        ourShader.setMat4("model", model);
                    }
                    else if(j == 0) {
                        glm::vec3 translation = matrix_to_ndc(i, j, SCR_WIDTH, SCR_HEIGHT);
                        model = glm::translate(model, translation);
                        ourShader.setMat4("model", model);
                    }
                }
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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

// Whenever a mouse button is pressed this callback function executes to
// append a new position at the location where the left mouse button was pressed
// so a primitive can be rendered at said position.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    int width, height;

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos, &ypos);
        grid[int(xpos)][int(SCR_HEIGHT-ypos)] = 1;
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

glm::vec3 matrix_to_ndc(int i, int j, const int width, const int height) {
    glm::vec3 point;
    point.x = (((float)i/width)*2)-1;
    point.y = 1.0*((((float)j/height)*2)-1);
    point.z = 0.0;
    return point;
}
