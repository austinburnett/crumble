#include "glad/glad.h"
#include "particle_system.hpp"
#include "grid.hpp"
#include "imgui.h"
#include "particle.hpp"
#include "particle_types.hpp"
#include <OpenGL/gl.h>

extern Grid GRID;
extern const unsigned int COLUMNS;
extern const unsigned int ROWS;

int ParticleSystem::active_particle = SandParticle::id;

ParticleSystem::ParticleSystem() {
}

ParticleSystem::~ParticleSystem() {
    G_SHOULD_THREAD_RUN = false;
    G_WORKER_THREAD.join();
}

void ParticleSystem::draw(unsigned int VAO, Shader& shader) {
    glBindVertexArray(VAO);
    shader.use();

    int instance_count = 0;

    // Render all the particles to the framebuffer.
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            if(GRID.at(i, j) != NULL && !GRID.at(i, j)->has_been_drawn) {
                glm::mat4 model(1.0f);
                GRID.at(i, j)->has_been_drawn = true;
                glm::vec3 color = GRID.at(i, j)->get_color();
                GRID.at(i, j)->update(i, j, GRID);

                glm::vec3 translation = grid_to_ndc(i, j, ROWS, COLUMNS);
                translations_[instance_count] = translation;
                colors_[instance_count] = color;
                ++instance_count;
            }
        }
    }
    gen_instanced_arrays_of_size(instance_count);

    if(instance_count > 0)
        glDrawArraysInstanced(GL_POINTS, 0, 1, instance_count);

    // Reset each particle's state so its only updated once per frame.
    reset_has_been_drawn_flags(GRID);

    glBindVertexArray(0);
}

void ParticleSystem::gen_instanced_arrays_of_size(int instance_count) {
    unsigned int instance_vbos[2];
    glGenBuffers(2, instance_vbos);

    // The translations instanced array.
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(glm::vec3),
                 &translations_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);

    // The colors instanced array.
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(glm::vec3),
                 &colors_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


glm::vec3 grid_to_ndc(int i, int j, const int width, const int height) {
    glm::vec3 point;
    point.x = (((float)i/width)*2)-1;
    point.y = 1.0*((((float)j/height)*2)-1);
    point.z = 0.0;
    return point;
}

void plot_particles_in_grid(GLFWwindow* window) {
    static double xpos, ypos;

    while(G_SHOULD_THREAD_RUN) {
        if(G_IS_THREAD_READY) {
            glfwGetCursorPos(window, &xpos, &ypos);
            if(int(xpos) >= 0 && int(COLUMNS-ypos) >= 0)
                if(int(xpos) < ROWS && int(COLUMNS-ypos) < COLUMNS) {
                    Particle* particle;
                    switch(ParticleSystem::active_particle) {
                        case(ParticleType::WATER):
                            particle = new WaterParticle();
                            break;
                        case(ParticleType::SAND):
                            particle = new SandParticle();
                            break;
                        case(ParticleType::WALL):
                            particle = new WallParticle();
                            break;
                        case(ParticleType::SMOKE):
                            particle = new SmokeParticle();
                            break;
                        case(ParticleType::WOOD):
                            particle = new WoodParticle();
                            break;
                        case(ParticleType::FIRE):
                            particle = new FireParticle();
                            break;
                        case(ParticleType::STEAM):
                            particle = new SteamParticle();
                            break;
                    }
                    // Flip the cursor's y-position such that it increases upwards.
                    GRID.at(int(xpos), int(COLUMNS-ypos)) = particle;
                    particle = nullptr;
                }
        }
    }
}

void display_particle_options_menu(double frame_time) {
    ImGuiWindowFlags imgui_window_flags = 0;
    bool* p_open = NULL;
    imgui_window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::Begin("Particle Choices", p_open, imgui_window_flags);

    ImGui::Text("Frame time: %f", frame_time);
    
    ImGui::RadioButton(SandParticle::name.c_str(),
                       &ParticleSystem::active_particle,
                       ParticleType::SAND);
    ImGui::RadioButton(WaterParticle::name.c_str(),
                       &ParticleSystem::active_particle,
                       ParticleType::WATER);
    ImGui::RadioButton(WallParticle::name.c_str(),
                       &ParticleSystem::active_particle,
                       ParticleType::WALL);
    ImGui::RadioButton(SmokeParticle::name.c_str(),
                       &ParticleSystem::active_particle,
                       ParticleType::SMOKE);
    ImGui::RadioButton(WoodParticle::name.c_str(),
                       &ParticleSystem::active_particle,
                       ParticleType::WOOD);
    ImGui::RadioButton(FireParticle::name.c_str(),
                       &ParticleSystem::active_particle,
                       ParticleType::FIRE);
    ImGui::RadioButton(SteamParticle::name.c_str(),
                       &ParticleSystem::active_particle,
                       ParticleType::STEAM);

    ImGui::End();
}

// Process all input every frame
void ParticleSystem::process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
