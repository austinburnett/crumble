#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <csignal>
#include <thread>

#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "grid.hpp"

inline bool        G_KEEP_THREAD_RUNNING = true;
inline bool        G_DO_WORK_IN_THREAD = false;
inline std::thread G_WORKER_THREAD;

// Manages the application's state, fills
// the framebuffer, and inits the dependencies.
class ParticleSystem {
public:
    ParticleSystem(GLFWwindow* window);
    ~ParticleSystem();

    void draw(const unsigned int VAO, Shader& shader);
    void process_input(GLFWwindow *window);

public:
    static int active_particle;
    static int s_particle_size;

private:
    // Creates multiple instanced arrays with the size specified.
    void gen_instanced_arrays_of_size(int instance_count);

private:
    // The size of this array is the max particle limit for the grid size
    // 550 x 550. This buffer stores the translations for each particle.
    glm::vec3 translations_[302500];
    glm::vec3 colors_[302500];
};

//-------------------
// Utility Functions
//-------------------

// Displays a menu consisting of different particles types to render.
//void display_particle_options_menu();
void display_particle_options_menu(double frame_time);


// This plots particles in the grid corresponding to the cursor's location
// which are in screen coordinates [0, 0], is in the top-left whereas the
// position [0, 0] in the grid corresponds to the bottom-left corner.
void plot_particles_in_grid(GLFWwindow* window);

void plot(Cell cell, const int amount, Particle* particles[]);
void plot_1x1(Cell cell, Particle* particle);
void plot_2x2(Cell cell, Particle* particles[]);
void plot_4x4(Cell cell, Particle* particles[]);

#endif
