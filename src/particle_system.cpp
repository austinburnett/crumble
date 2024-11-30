#include <imgui/imgui.h>
#include <glad/glad.h>
#include <OpenGL/gl.h>

#include "grid.hpp"
#include "particle.hpp"
#include "particle_sizes.hpp"
#include "particle_system.hpp"
#include "particle_types.hpp"


extern Grid GRID;
extern const unsigned int COLUMNS;
extern const unsigned int ROWS;

int ParticleSystem::active_particle = SandParticle::id;
int ParticleSystem::s_particle_size = 0;

ParticleSystem::ParticleSystem(GLFWwindow* window) {
    G_WORKER_THREAD = std::thread(plot_particles_in_grid, window);
}

ParticleSystem::~ParticleSystem() {
    G_KEEP_THREAD_RUNNING = false;
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
    GRID.reset_has_been_drawn_flags();

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

void plot_particles_in_grid(GLFWwindow* window) {
    static double xpos, ypos;

    while(G_KEEP_THREAD_RUNNING) {
        if(G_DO_WORK_IN_THREAD) {
            glfwGetCursorPos(window, &xpos, &ypos);
            if(int(xpos) >= 0 && int(COLUMNS-ypos) >= 0)
                if(int(xpos) < ROWS && int(COLUMNS-ypos) < COLUMNS) {
                    int amount_to_plot;
                    switch(ParticleSystem::s_particle_size) {
                        case Size::SIZE_ZERO:
                            amount_to_plot = 1; // 1x1
                            break;
                        case Size::SIZE_ONE:
                            amount_to_plot = 4; // 2x2
                            break;
                        case Size::SIZE_TWO:
                            amount_to_plot = 16; // 4x4
                            break;
                    }

                    Particle* particles[amount_to_plot];
                    switch(ParticleSystem::active_particle) {
                        int i;
                        case(ParticleType::WATER): {
                            for(i = 0; i < amount_to_plot; ++i)
                                particles[i] = new WaterParticle();
                            break;
                        }
                        case(ParticleType::SAND): {
                            for(i = 0; i < amount_to_plot; ++i)
                                particles[i] = new SandParticle();
                            break;
                        }
                        case(ParticleType::WALL): {
                            for(i = 0; i < amount_to_plot; ++i)
                                particles[i] = new WallParticle();
                            break;
                        }
                        case(ParticleType::SMOKE): {
                            for(i = 0; i < amount_to_plot; ++i)
                                particles[i] = new SmokeParticle();
                            break;
                        }
                        case(ParticleType::WOOD): {
                            for(i = 0; i < amount_to_plot; ++i)
                                particles[i] = new WoodParticle();
                            break;
                        }
                        case(ParticleType::FIRE): {
                            for(i = 0; i < amount_to_plot; ++i)
                                particles[i] = new FireParticle();
                            break;
                        }
                        case(ParticleType::STEAM): {
                            for(i = 0; i < amount_to_plot; ++i)
                                particles[i] = new SteamParticle();
                            break;
                        }
                    }

                    // Flip the cursor's y-position such that it increases upwards.
                    plot(Cell(int(xpos), int(COLUMNS-ypos)), amount_to_plot, particles);

                    for(int i = 0; i < amount_to_plot; ++i)
                        particles[i] = nullptr;
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

    ImGui::RadioButton("Size 0", &ParticleSystem::s_particle_size, Size::SIZE_ZERO);
    ImGui::RadioButton("Size 1", &ParticleSystem::s_particle_size, Size::SIZE_ONE);
    ImGui::RadioButton("Size 2", &ParticleSystem::s_particle_size, Size::SIZE_TWO);
    ImGui::NewLine();
    
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
    ImGui::NewLine();
    if(ImGui::Button("Clear"))
        GRID.clear();

    ImGui::End();
}

// Process all input every frame
void ParticleSystem::process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void plot(Cell cell, const int amount, Particle* particles[]) {
    switch(amount) {
        case 1:
            plot_1x1(cell, particles[0]);
            break;
        case 4:
            plot_2x2(cell, particles);
            break;
        case 16:
            plot_4x4(cell, particles);
            break;
    }
}

void plot_1x1(Cell cell, Particle* particle) {
    GRID.insert(cell, particle);
}

void plot_2x2(Cell cell, Particle* particles[]) {
    GRID.insert(cell,              particles[0]);
    GRID.insert(cell.right(),      particles[1]);
    GRID.insert(cell.down(),       particles[2]);
    GRID.insert(cell.down_right(), particles[3]);
}

void plot_4x4(Cell cell, Particle* particles[]) {
    // Top row
    GRID.insert(cell,               particles[0]);
    GRID.insert(cell.x + 1, cell.y, particles[1]);
    GRID.insert(cell.x + 2, cell.y, particles[2]);
    GRID.insert(cell.x + 3, cell.y, particles[3]);

    // Middle rows
    GRID.insert(cell.x,     cell.y - 1, particles[4]);
    GRID.insert(cell.x + 1, cell.y - 1, particles[5]);
    GRID.insert(cell.x + 2, cell.y - 1, particles[6]);
    GRID.insert(cell.x + 3, cell.y - 1, particles[7]);

    GRID.insert(cell.x,     cell.y - 2, particles[8]);
    GRID.insert(cell.x + 1, cell.y - 2, particles[9]);
    GRID.insert(cell.x + 2, cell.y - 2, particles[10]);
    GRID.insert(cell.x + 3, cell.y - 2, particles[11]);

    // Bottom row
    GRID.insert(cell.x,     cell.y - 3, particles[12]);
    GRID.insert(cell.x + 1, cell.y - 3, particles[13]);
    GRID.insert(cell.x + 2, cell.y - 3, particles[14]);
    GRID.insert(cell.x + 3, cell.y - 3, particles[15]);
}
