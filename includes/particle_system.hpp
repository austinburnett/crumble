// A header file that contains the declaractions
// for the ParticleSystem class and its related
// utility functions.

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

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
#include <csignal>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "square.hpp"
#include "particle.hpp"
#include "grid.hpp"
#include "../glfw_callbacks.cpp"

static bool SHOULD_THREAD_RUN = true;

// The ParticleSystem class is responsible for
// initializng all dependencies needed to render
// and cleaning up those dependencies whenever the
// object goes out of scope.
class ParticleSystem {
public:
    ParticleSystem();

    ~ParticleSystem();

GLFWwindow* window;
};

//-------------------
// Utility Functions
//-------------------

// Displays a menu consisting of different particles types to render.
//void display_particle_options_menu();
void display_particle_options_menu();

// Convert from the grid with the ranges [0, ROWS] and [0, COLUMNS] to ndc.
// Opengl expects vertices between [-1, 1] and a y-axis pointing up.
//glm::vec3 grid_to_ndc(int i, int j, const int width, const int height);
glm::vec3 grid_to_ndc(int i, int j, const int width, const int height);

// This plots particles in the grid corresponding to the cursor's location
// which are in screen coordinates [0, 0], is in the top-left whereas the
// position [0, 0] in the grid corresponds to the bottom-left corner.
void plot_particles_in_grid(GLFWwindow* window);

#endif