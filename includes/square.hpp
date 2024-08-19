#ifndef SQUARE_H
#define SQUARE_H

#include <glad/glad.h>
#include <OpenGL/OpenGL.h>

class Square {
public:
    // Constructor
    Square();

    // Destructor
    ~Square();

    unsigned int VBO, VAO, EBO;
private:
};

#endif // !SQUARE_H
