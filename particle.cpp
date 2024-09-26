// This module contains implementations for the
// Particle abstract base class and its derived classes.

#include <random>

#include "includes/particle.hpp"

//------------------------------
// Particle ABC
//------------------------------
glm::vec3 Particle::get_color() const {
    return glm::vec3(1.0f, 1.0f, 1.0f); 
}

//------------------------------
// Sand Particle
//------------------------------
void SandParticle::update(const int i, const int j, Grid& grid)
const {
    // Move particle down one block if nothing is there
    if(grid.at(i, j-1) == NULL) {
        grid.at(i, j-1) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
    // Move particle down and left by one block if nothing is there
    else if(grid.at(i-1, j-1) == NULL) {
        grid.at(i-1, j-1) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
    // Move particle down and right by one block if nothing is there
    else if(grid.at(i+1, j-1) == NULL) {
        grid.at(i+1, j-1) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
}

glm::vec3 SandParticle::get_color() const {
    return glm::vec3(0.79f, 0.74f, 0.58f); 
}

//------------------------------
// Water Particle
//------------------------------
void WaterParticle::update(const int i, const int j, Grid& grid)
const {
    // Generate a random number between [1, 100]
    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib_100(1,100);
    int rand_num = distrib_100(rand_generator);

    // Move particle down one block if nothing is there
    if(j > 0 && grid.at(i, j-1) == NULL) {
        grid.at(i, j-1) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
    // Move particle down and left by one block if nothing is there
    else if(i > 0 && j > 0 && grid.at(i-1, j-1) == NULL) {
        grid.at(i-1, j-1) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
    // Move particle down and right by one block if nothing is there
    else if(i < ROWS-1 && j > 0 && grid.at(i+1, j-1) == NULL) {
        grid.at(i+1, j-1) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
    // Move particle left one block if nothing is there
    else if(i > 0 && rand_num < 50 && grid.at(i-1, j) == NULL) {
        grid.at(i-1, j) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
    // Move particle right one block if nothing is there
    else if(i < ROWS-1 && rand_num > 50 && grid.at(i+1, j) == NULL) {
        grid.at(i+1, j) = grid.at(i, j);
        grid.at(i, j) = NULL;
    }
}

glm::vec3 WaterParticle::get_color() const {
    return glm::vec3(0.0f, 0.0f, 1.0f); 
}

//---------------------
// Particle Class Utilities
//---------------------
void reset_has_been_drawn_flags(Grid &grid) {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            if(grid.at(i, j) != NULL)
                grid.at(i, j)->has_been_drawn = false;
        }
    }
}
