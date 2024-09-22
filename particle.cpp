// This module contains implementations for the
// Particle abstract base class and its derived classes.

#include "includes/particle.hpp"

glm::vec3 Particle::get_color() const {
    return glm::vec3(1.0f, 1.0f, 1.0f); 
}

void SandParticle::update(int i, int j, Particle* (&grid)[550][550])
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

void WaterParticle::update(int i, int j, Particle* (&grid)[550][550])
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
    // Move particle right one block if nothing is there
    else if(grid[i+1][j] == NULL) {
        grid[i+1][j] = grid[i][j];
        grid[i][j] = NULL;
    }
    // Move particle left one block if nothing is there
    else if(grid[i-1][j] == NULL) {
        grid[i-1][j] = grid[i][j];
        grid[i][j] = NULL;
    }
}

glm::vec3 WaterParticle::get_color() const {
    return glm::vec3(0.0f, 0.0f, 1.0f); 
}
