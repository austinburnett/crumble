#ifndef CAMERA_H
#define CAMERA_H

#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

// Default Camera Settings
static float YAW = -90.0f;
static float PITCH = 0.0f;
static float FOV = 45.0f;
static float CAMERA_SPEED = 2.5;

// Usage:
// The intended use for this class
// is to be created prior to the render loop
// and call the process_user_input() method every frame
// 
// FlyCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// camera.update_direction();
// view = camera.get_lookAt();
// ourShader.setMat4("view", view);
class AbstractCamera {
public:
    // Constructor
    AbstractCamera() {}

    // Destructor
    virtual ~AbstractCamera() {}

    // Returns the normalized direction the camera is facing
    // Each component is in the range [-1, 1]
    virtual glm::vec3 direction() const = 0;

    // Updates the direction of the camera
    virtual void update_direction() = 0;

    // Returns the lookAt matrix
    virtual glm::mat4 get_lookAt() const = 0;

    // Update camera state through keyboard input
    virtual void process_key_input(GLFWwindow* window, const float delta_time)
                                                                          = 0;

    glm::vec3 front; // Direction the camera is looking at
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Camera up direction vector
private:
};

// Camera implemented using Euler's angles
class FlyCamera: public AbstractCamera {
public:
    // FlyCamera scalar constructor
    FlyCamera(float x, float y, float z) {
        pos.x = x; pos.y = y; pos.z = z;
    }

    // FlyCamera vector constructor
    FlyCamera(glm::vec3 position): pos(position) {}
    
    // Returns the normalized direction the camera is facing
    // Each component is in the range [-1, 1]
    glm::vec3 direction() const override {
        return glm::normalize(front);
    }

    // Updates the direction vector using Eulers angles
    // Usage: call before get_lookAt()
    void update_direction() override {
        front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
        front.y = sin(glm::radians(PITCH));
        front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
    }

    // Returns a matrix used to construct the view transformation matrix
    glm::mat4 get_lookAt() const override {
        //return glm::lookAt(pos, pos + direction(), up);
        return gen_lookAt(pos, pos + direction(), up);
    }

    // Usage: inside a render loop, updates camera position on keyboard input
    void process_key_input(GLFWwindow* window, const float delta_time) override {
        CAMERA_SPEED = 2.5f * delta_time;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pos += (CAMERA_SPEED * direction());
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pos -= (CAMERA_SPEED * direction());
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pos -= glm::normalize(glm::cross(direction(), up)) *
                                                    CAMERA_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pos += glm::normalize(glm::cross(direction(), up)) *
                                                    CAMERA_SPEED;
        }
    }

    // The position of the Camera
    glm::vec3 pos;

private:
    // Generates a 4x4 transformation matrix that 
    // outputs the vertices as seen from the camera
    glm::mat4 gen_lookAt(glm::vec3 cam_pos, glm::vec3 target, glm::vec3 world_up) const {
        glm::vec3 zaxis = glm::normalize(cam_pos - target);
        glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(world_up),
                                                    zaxis));

        glm::vec3 yaxis = glm::cross(zaxis, xaxis);
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0][0] = xaxis.x;
        rotation[1][0] = xaxis.y;
        rotation[2][0] = xaxis.z;
        rotation[0][1] = yaxis.x;
        rotation[1][1] = yaxis.y;
        rotation[2][1] = yaxis.z;
        rotation[0][2] = zaxis.x;
        rotation[1][2] = zaxis.y;
        rotation[2][2] = zaxis.z;

        glm::mat4 translation = glm::mat4(1.0f);
        translation[3][0] = -cam_pos.x;
        translation[3][1] = -cam_pos.y;
        translation[3][2] = -cam_pos.z;

        return rotation * translation;
    }
};

// Camera that stays on the xz plane
class FpsCamera: public AbstractCamera {
public:
    // FlyCamera scalar constructor
    FpsCamera(float x, float y, float z) {
        pos.x = x; pos.y = y; pos.z = z;
    }

    // FlyCamera vector constructor
    FpsCamera(glm::vec3 position): pos(position) {}

    // Returns the normalized direction the camera is facing
    // Each component is in the range [-1, 1]
    glm::vec3 direction() const override {
        return glm::normalize(front);
    }

    // Updates the direction vector using Eulers angles
    // Usage: call before get_lookAt()
    void update_direction() override {
        front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
        front.y = sin(glm::radians(PITCH));
        front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
    }

    // Returns a matrix used to construct the view transformation matrix
    glm::mat4 get_lookAt() const override {
        return glm::lookAt(pos, pos + direction(), up);
    }

    // Usage: inside a render loop, updates camera position on keyboard input
    void process_key_input(GLFWwindow* window, const float delta_time) override {
        CAMERA_SPEED = 2.5f * delta_time;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            auto dir = direction();
            pos.x += (CAMERA_SPEED * dir.x);
            pos.z += (CAMERA_SPEED * dir.z);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            auto dir = direction();
            pos.x -= (CAMERA_SPEED * dir.x);
            pos.z -= (CAMERA_SPEED * dir.z);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pos -= glm::normalize(glm::cross(direction(), up)) *
                CAMERA_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pos += glm::normalize(glm::cross(direction(), up)) *
                CAMERA_SPEED;
        }
        pos.y = 0.0f;
    }

    // The position of the Camera
    glm::vec3 pos;

private:

};

// For usage inside a callback that occurs when input is received
// via the scroll wheel
inline void process_scroll_input(double y_offset) {
    FOV -= static_cast<float>(y_offset);

    if(FOV < 1.0f) {
        FOV = 1.0f;
    }
    if(FOV > 45.0f) {
        FOV = 45.0f;
    }
}

// For usage inside a callback that occurs when input is received
// via mouse movement
inline void process_mouse_input(float x_offset, float y_offset) {
    static const float sensitivity = 0.009f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    YAW += x_offset;
    PITCH += y_offset;

    if(PITCH > 89.0f) {
        PITCH = 89.0f;
    }
    else if(PITCH < -89.0f) {
        PITCH = -89.0f;
    }
}

#endif
