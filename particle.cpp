// This module contains implementations for the
// Particle abstract base class and its derived classes.

#include "includes/particle.hpp"
#include <random>

glm::vec3 Particle::get_color() const {
    return glm::vec3(1.0f, 1.0f, 1.0f); 
}

void SandParticle::update(const int i, const int j, Particle* (&grid)[550][550])
const {
    // Move particle down one block if nothing is there
    if(grid[i][j-1] == NULL) {
        grid[i][j-1] = grid[i][j];
        grid[i][j] = NULL;
    }
    // Move particle down and left by one block if nothing is there
    else if(grid[i-1][j-1] == NULL) {
        grid[i-1][j-1] = grid[i][j];
        grid[i][j] = NULL;
    }
    // Move particle down and right by one block if nothing is there
    else if(grid[i+1][j-1] == NULL) {
        grid[i+1][j-1] = grid[i][j];
        grid[i][j] = NULL;
    }
}

glm::vec3 SandParticle::get_color() const {
    return glm::vec3(0.79f, 0.74f, 0.58f); 
}

void WaterParticle::update(const int i, const int j, Particle* (&grid)[550][550])
const {
    // Generate a random number between [1, 100]
    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib_100(1,100);
    int rand_num = distrib_100(rand_generator);

    // Move particle down one block if nothing is there
    if(j > 0 && grid[i][j-1] == NULL) {
        grid[i][j-1] = grid[i][j];
        grid[i][j] = NULL;
    }
    // Move particle down and left by one block if nothing is there
    else if(i > 0 && j > 0 && grid[i-1][j-1] == NULL) {
        grid[i-1][j-1] = grid[i][j];
        grid[i][j] = NULL;
    }
    // Move particle down and right by one block if nothing is there
    else if(i < 550 && j > 0 && grid[i+1][j-1] == NULL) {
        grid[i+1][j-1] = grid[i][j];
        grid[i][j] = NULL;
    }
    // Move particle left one block if nothing is there
    else if(i > 0 && rand_num < 50 && grid[i-1][j] == NULL) {
        grid[i-1][j] = grid[i][j];
        grid[i][j] = NULL;
    }
    // Move particle right one block if nothing is there
    else if(i < 550 && rand_num > 50 && grid[i+1][j] == NULL) {
        grid[i+1][j] = grid[i][j];
        grid[i][j] = NULL;
    }
}

glm::vec3 WaterParticle::get_color() const {
    return glm::vec3(0.0f, 0.0f, 1.0f); 
}
