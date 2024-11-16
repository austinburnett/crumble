#include <random>
#include <iostream>

#include "particle.hpp"
#include "particle_sys/grid.hpp"


//------------------------------
// Sand Particle
//------------------------------
const std::string SandParticle::name = "Sand";

void SandParticle::update(const int i, const int j, Grid& grid) {
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

Color3 SandParticle::get_color() const {
    return Color3(0.79f, 0.74f, 0.58f); 
}

bool SandParticle::is_flammable() const {
    return false;
}

//------------------------------
// Water Particle
//------------------------------
const std::string WaterParticle::name = "Water";

void WaterParticle::update(const int i, const int j, Grid& grid) {
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
    else if(i-get_dispersion_rate() > 0 && grid.is_cell_empty(i-get_dispersion_rate(), j) && direction_factor <= 5) {
        grid.swap(i, j, i - get_dispersion_rate(), j);
    }
    // Move particle right one block if nothing is there
    else if(i+get_dispersion_rate() < ROWS && grid.is_cell_empty(i+get_dispersion_rate(), j) && direction_factor > 5) {
        grid.swap(i, j, i+get_dispersion_rate(), j);
    }
}

Color3 WaterParticle::get_color() const {
    return Color3(0.0f, 0.0f, 1.0f); 
}

bool WaterParticle::is_flammable() const {
    return false;
}

int WaterParticle::get_dispersion_rate() const {
    return 5;
}

//------------------------------
// Wall Particle
//------------------------------
const std::string WallParticle::name = "Wall";

void WallParticle::update(const int i, const int j, Grid& grid) {
    // The WallParticle doesn't move.
}

Color3 WallParticle::get_color() const {
    return Color3(0.1f, 0.1f, 0.1f); 
}

bool WallParticle::is_flammable() const {
    return false;
}

//------------------------------
// Smoke Particle
//------------------------------
const std::string SmokeParticle::name = "Smoke";

void SmokeParticle::update(const int i, const int j, Grid& grid) {
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

Color3 SmokeParticle::get_color() const {
    return Color3(0.5f, 0.5f, 0.5f); 
}

bool SmokeParticle::is_flammable() const {
    return false;
}

//------------------------------
// Wood Particle
//------------------------------
const std::string WoodParticle::name = "Wood";

void WoodParticle::update(const int i, const int j, Grid& grid) {
    // The WoodParticle doesn't move.
}

Color3 WoodParticle::get_color() const {
    return Color3(0.59f, 0.29f, 0.0f); 
}

bool WoodParticle::is_flammable() const {
    return true;
}

//------------------------------
// Fire Particle
//------------------------------
const std::string FireParticle::name = "Fire";

void FireParticle::update(const int i, const int j, Grid& grid) {
    lifetime_left_--;
    delay_until_inflamed_--;

    if(lifetime_left_ <= 0) {
        delete grid.at(i, j);
        grid.at(i, j) = NULL;
        return;
    }

    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib_100(1,100);
    int flame_expansion_chance = distrib_100(rand_generator);
    constexpr int THRESHOLD = 90;

    if(flame_expansion_chance > THRESHOLD) {
        if(j < COLUMNS-1 && i < ROWS-1 && grid.is_cell_empty(i+1, j+1)) {
            grid.at(i + 1, j + 1) = new FireParticle();
        }
        else if(j < COLUMNS-1 && i > 0 && grid.is_cell_empty(i-1, j+1)) {
            grid.at(i - 1, j + 1) = new FireParticle();
        }
    }

    if(delay_until_inflamed_ <= 0) {
        // Check if there are flammable objects in its surroundings.
        if(j < COLUMNS-1 && !grid.is_cell_empty(i, j+1) &&
            grid.at(i, j+1)->is_flammable()) {
            delete grid.at(i, j + 1);
            Particle* particle = new FireParticle();
            grid.at(i, j + 1) = particle;
        }
        else if(j > 0 && !grid.is_cell_empty(i, j-1) && grid.at(i, j-1)->is_flammable()) {
            delete grid.at(i, j-1);
            Particle* particle = new FireParticle();
            grid.at(i, j-1) = particle;
        }
        else if(i > 0 && !grid.is_cell_empty(i-1, j) && grid.at(i-1, j)->is_flammable()) {
            delete grid.at(i-1, j);
            Particle* particle = new FireParticle();
            grid.at(i-1, j) = particle;
        }
        else if(i < ROWS-1 && !grid.is_cell_empty(i+1, j) && grid.at(i+1, j)->is_flammable()) {
            delete grid.at(i+1, j);
            Particle* particle = new FireParticle();
            grid.at(i+1, j) = particle;
        }
        // Check the diagonals.
        else if(i > 0 && j > 0 && !grid.is_cell_empty(i-1, j-1) && grid.at(i-1, j-1)->is_flammable()) {
            delete grid.at(i-1, j-1);
            Particle* particle = new FireParticle();
            grid.at(i-1, j-1) = particle;
        }
        else if(i < ROWS-1 && j > 0 && !grid.is_cell_empty(i+1, j-1) && grid.at(i+1, j-1)->is_flammable()) {
            delete grid.at(i+1, j-1);
            Particle* particle = new FireParticle();
            grid.at(i+1, j-1) = particle;
        }
        else if(i < ROWS-1 && j < COLUMNS-1 && !grid.is_cell_empty(i+1, j+1) && grid.at(i+1, j+1)->is_flammable()) {
            delete grid.at(i+1, j+1);
            Particle* particle = new FireParticle();
            grid.at(i+1, j+1) = particle;
        }
        else if(i > 0 && j < COLUMNS-1 && !grid.is_cell_empty(i-1, j+1) && grid.at(i-1, j+1)->is_flammable()) {
            delete grid.at(i-1, j+1);
            Particle* particle = new FireParticle();
            grid.at(i-1, j+1) = particle;
        }
        // Reset back to its original value.
        delay_until_inflamed_ = 5;
    }
}

Color3 FireParticle::get_color() const {
    if(lifetime_left_ <= 5)
        return Color3(1.0f, 0.6f, 0.0f);
    return Color3(1.00f, 0.0f, 0.0f); 
}

bool FireParticle::is_flammable() const {
    return false;
}
