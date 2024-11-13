#include <random>

#include "particle.hpp"


//------------------------------
// Sand Particle
//------------------------------
const std::string SandParticle::name = "Sand";

void SandParticle::update(const int i, const int j, Grid& grid)
const {
    // Move particle down one block if nothing is there
    if(j > 0 && grid.is_cell_empty(i, j-1)) {
        grid.swap(i, j, i, j - 1);
    }
    // Move particle down and left by one block if nothing is there
    else if(j > 0 && i > 0 && grid.is_cell_empty(i-1, j-1)) {
        grid.swap(i, j, i - 1, j - 1);
    }
    // Move particle down and right by one block if nothing is there
    else if(j > 0 && i < COLUMNS-1 && grid.is_cell_empty(i+1, j-1)) {
        grid.swap(i, j, i + 1, j - 1);
    }
}

glm::vec3 SandParticle::get_color() const {
    return glm::vec3(0.79f, 0.74f, 0.58f); 
}

//------------------------------
// Water Particle
//------------------------------
const std::string WaterParticle::name = "Water";

void WaterParticle::update(const int i, const int j, Grid& grid)
const {
    // Generate a random number between [1, 10]
    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib_10(1,10);
    int direction_factor = distrib_10(rand_generator);

    // Move particle down one block if nothing is there
    if(j > 0 && grid.is_cell_empty(i, j-1)) {
        grid.swap(i, j, i, j - 1);
    }
    // Move particle down and left by one block if nothing is there
    else if(i > 0 && j > 0 && grid.is_cell_empty(i-1, j-1)) {
        grid.swap(i, j, i-1, j-1);
    }
    // Move particle down and right by one block if nothing is there
    else if(i < ROWS-1 && j > 0 && grid.is_cell_empty(i+1, j-1)) {
        grid.swap(i, j, i+1, j-1);
    }
    // Move particle left one block if nothing is there
    else if(i > 0 && grid.is_cell_empty(i-1, j) && direction_factor <= 5) {
        grid.swap(i, j, i - 1, j);
    }
    // Move particle right one block if nothing is there
    else if(i < ROWS-1 && grid.is_cell_empty(i+1, j) && direction_factor > 5) {
        grid.swap(i, j, i+1, j);
    }
}

glm::vec3 WaterParticle::get_color() const {
    return glm::vec3(0.0f, 0.0f, 1.0f); 
}

//------------------------------
// Wall Particle
//------------------------------
const std::string WallParticle::name = "Wall";

void WallParticle::update(const int i, const int j, Grid& grid)
const {
    // The WallParticle doesn't move.
}

glm::vec3 WallParticle::get_color() const {
    return glm::vec3(0.1f, 0.1f, 0.1f); 
}

//------------------------------
// Smoke Particle
//------------------------------
const std::string SmokeParticle::name = "Smoke";

void SmokeParticle::update(const int i, const int j, Grid& grid)
const {
    // Generate a random number between [1, 10]
    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib_10(1,10);
    int direction_factor = distrib_10(rand_generator);

    // Move particle up one block if nothing is there.
    if(j < COLUMNS-1 && grid.is_cell_empty(i, j+1)) {
        grid.swap(i, j, i, j + 1);
    }
    // Move particle up and left by one block if nothing is there
    else if(i > 0 && j < COLUMNS-1 && grid.is_cell_empty(i-1, j+1)) {
        grid.swap(i, j, i - 1, j + 1);
    }
    // Move particle up and right by one block if nothing is there
    else if(i < ROWS-1 && j < COLUMNS-1 && grid.is_cell_empty(i+1, j+1)) {
        grid.swap(i, j, i + 1, j + 1);
    }
    // Move particle left one block if nothing is there
    else if(i > 0 && grid.is_cell_empty(i-1, j) && direction_factor <= 5) {
        grid.swap(i, j, i - 1, j);
    }
    // Move particle right one block if nothing is there
    else if(i < ROWS-1 && grid.is_cell_empty(i+1, j) && direction_factor > 5) {
        grid.swap(i, j, i + 1, j);
    }
}

glm::vec3 SmokeParticle::get_color() const {
    return glm::vec3(0.5f, 0.5f, 0.5f); 
}

//------------------------------
// Wood Particle
//------------------------------
const std::string WoodParticle::name = "Wood";

void WoodParticle::update(const int i, const int j, Grid& grid)
const {
    // The WoodParticle doesn't move.
}

glm::vec3 WoodParticle::get_color() const {
    return glm::vec3(0.59f, 0.29f, 0.0f); 
}

//------------------------------
// Fire Particle
//------------------------------
const std::string FireParticle::name = "Fire";

void FireParticle::update(const int i, const int j, Grid& grid)
const {
    // The FireParticle doesn't move.
}

glm::vec3 FireParticle::get_color() const {
    return glm::vec3(1.00f, 0.0f, 0.0f); 
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
