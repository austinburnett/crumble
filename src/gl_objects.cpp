#include <glad/glad.h>
#include <OpenGL/OpenGL.h>

#include "gl_objects.hpp"

// Constructor
Square::Square() {
    float vertices[] = {
         0.0f, 0.0f, 1.0f, // bottom-left
         0.01f, 0.0f, 1.0f, // bottom-right
         0.01f, 0.01f, 1.0f, // top-right

         0.0f, 0.01f, 1.0f, // top-left
    };

    int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


// Destructor
Square::~Square() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

Point::Point() {
    float vertices[] = {
         0.0f, 0.0f, 1.0f
    };

    glPointSize(2);

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // Set the newly generated objects as objects currently in use.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Create a new data store for the current buffer object.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO.
    glBindVertexArray(0);

    // The vertex buffer object can be deleted after the VAO is unbound
    // since the VAO will maintain a reference to the VBO.
    glDeleteBuffers(1, &VBO);
}

Point::~Point() {
    glDeleteVertexArrays(1, &VAO);
}
