#include <random>

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

bool SandParticle::is_affected_by(const std::string particle) const {
    return false;
}

Color3 SandParticle::get_color() const {
    return Color3(0.79f, 0.74f, 0.58f); 
}

//------------------------------
// Water Particle
//------------------------------
const std::string WaterParticle::name = "Water";

void WaterParticle::update(const int i, const int j, Grid& grid) {
    Cell curr_cell(i, j);

    // Generate a random number between [0, 1]
    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    constexpr int RIGHT_THRESHOLD = 1;

    std::uniform_int_distribution<std::mt19937::result_type> distrib_1(0, RIGHT_THRESHOLD);
    const int DIRECTION = distrib_1(rand_generator);

    // Move particle down one block if nothing is there
    if(j > 0 && grid.is_cell_empty(curr_cell.down())) {
        grid.swap(curr_cell, curr_cell.down());
    }
    // Move particle down and left by one block if nothing is there
    else if(i > 0 && j > 0 && grid.is_cell_empty(curr_cell.down_left())) {
        grid.swap(curr_cell, curr_cell.down_left());
    }
    // Move particle down and right by one block if nothing is there
    else if(i < ROWS-1 && j > 0 && grid.is_cell_empty(curr_cell.down_right())) {
        grid.swap(curr_cell, curr_cell.down_right());
    }
    // Move particle left if nothing is there
    else if(i-get_dispersion_rate() > 0 && DIRECTION < RIGHT_THRESHOLD) {
        grid.move_cell_left_until_blocked(Cell(i, j), get_dispersion_rate());
    }
    // Move particle right if nothing is there
    else if(i+get_dispersion_rate() < ROWS && DIRECTION >= RIGHT_THRESHOLD) {
        grid.move_cell_right_until_blocked(Cell(i, j), get_dispersion_rate());
    }
}

bool WaterParticle::is_affected_by(const std::string particle) const {
    if(particle == "fire") {
        return true;
    }
    return false;
}

Color3 WaterParticle::get_color() const {
    return Color3(0.0f, 0.0f, 1.0f);
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

bool WallParticle::is_affected_by(const std::string particle) const {
    return false;
}

Color3 WallParticle::get_color() const {
    //return Color3(0.1f, 0.1f, 0.1f); 
    return Color3(1.0f, 1.0f, 1.0f); 
}

//------------------------------
// Smoke Particle
//------------------------------
const std::string SmokeParticle::name = "Smoke";

void SmokeParticle::update(const int i, const int j, Grid& grid) {
    Cell curr_cell(i, j);
    lifetime_left_--;

    if(lifetime_left_ <= 0) {
        delete grid.at(curr_cell.x, curr_cell.y);
        grid.at(curr_cell.x, curr_cell.y) = NULL;
        return;
    }

    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib_100(1,100);
    int scatter_chance = distrib_100(rand_generator);

    if(j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up()) && will_go_up(scatter_chance)) {
        grid.swap(curr_cell, curr_cell.up());
    }
    else if(i > 0 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_left()) && will_go_up_left(scatter_chance)) {
        grid.swap(curr_cell, curr_cell.up_left());
    }
    else if(i < ROWS-1 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_right()) && will_go_up_right(scatter_chance)) {
        grid.swap(curr_cell, curr_cell.up_right());
    }
    else if(i > 0 && grid.is_cell_empty(curr_cell.left()) && scatter_chance <= 50) {
        grid.swap(curr_cell, curr_cell.left());
    }
    else if(i < ROWS-1 && grid.is_cell_empty(curr_cell.right()) && scatter_chance > 50) {
        grid.swap(curr_cell, curr_cell.right());
    }
}

bool SmokeParticle::is_affected_by(const std::string particle) const {
    return false;
}

Color3 SmokeParticle::get_color() const {
    return Color3(0.4f, 0.4f, 0.4f); 
}

bool SmokeParticle::will_go_up(int val) const {
    if(val <= up_chance) {
        return true;
    }
    return false;
}

bool SmokeParticle::will_go_up_left(int val) const {
    if(val > up_chance && val <= up_chance + up_left_chance) {
        return true;
    }
    return false;
}

bool SmokeParticle::will_go_up_right(int val) const {
    if(val > up_chance + up_left_chance) {
        return true;
    }
    return false;
}

//------------------------------
// Wood Particle
//------------------------------
const std::string WoodParticle::name = "Wood";

void WoodParticle::update(const int i, const int j, Grid& grid) {
    // The WoodParticle doesn't move.
}

bool WoodParticle::is_affected_by(const std::string particle) const {
    if(particle == "fire") {
        return true;
    }
    return false;
}

Color3 WoodParticle::get_color() const {
    return Color3(0.59f, 0.29f, 0.0f); 
}

//------------------------------
// Fire Particle
//------------------------------
const std::string FireParticle::name = "Fire";

void FireParticle::update(const int i, const int j, Grid& grid) {
    Cell curr_cell(i, j);
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
        if(j < COLUMNS-1 && i < ROWS-1 && grid.is_cell_empty(curr_cell.up_right())) {
            grid.at(i + 1, j + 1) = new FireParticle();
        }
        else if(j < COLUMNS-1 && i > 0 && grid.is_cell_empty(curr_cell.up_left())) {
            grid.at(i - 1, j + 1) = new FireParticle();
        }
    }

    if(delay_until_inflamed_ <= 0) {
        // Reset back to its original value.
        delay_until_inflamed_ = 5;

        // Check if there are flammable objects in its surroundings.
        if(j < COLUMNS-1 && !grid.is_cell_empty(curr_cell.up()) && grid.at(i, j+1)->is_affected_by("fire")) {
            delete grid.at(i, j + 1);
            Particle* particle = new FireParticle();
            grid.at(i, j + 1) = particle;
        }
        else if(j > 0 && !grid.is_cell_empty(curr_cell.down()) && grid.at(i, j-1)->is_affected_by("fire")) {
            delete grid.at(i, j-1);
            Particle* particle = new FireParticle();
            grid.at(i, j-1) = particle;
        }
        else if(i > 0 && !grid.is_cell_empty(curr_cell.left()) && grid.at(i-1, j)->is_affected_by("fire")) {
            delete grid.at(i-1, j);
            Particle* particle = new FireParticle();
            grid.at(i-1, j) = particle;
        }
        else if(i < ROWS-1 && !grid.is_cell_empty(curr_cell.right()) && grid.at(i+1, j)->is_affected_by("fire")) {
            delete grid.at(i+1, j);
            Particle* particle = new FireParticle();
            grid.at(i+1, j) = particle;
        }
        // Check the diagonals.
        else if(i > 0 && j > 0 && !grid.is_cell_empty(curr_cell.down_left()) && grid.at(i-1, j-1)->is_affected_by("fire")) {
            delete grid.at(i-1, j-1);
            Particle* particle = new FireParticle();
            grid.at(i-1, j-1) = particle;
        }
        else if(i < ROWS-1 && j > 0 && !grid.is_cell_empty(curr_cell.down_right()) && grid.at(i+1, j-1)->is_affected_by("fire")) {
            delete grid.at(i+1, j-1);
            Particle* particle = new FireParticle();
            grid.at(i+1, j-1) = particle;
        }
        else if(i < ROWS-1 && j < COLUMNS-1 && !grid.is_cell_empty(curr_cell.up_right()) && grid.at(i+1, j+1)->is_affected_by("fire")) {
            delete grid.at(i+1, j+1);
            Particle* particle = new FireParticle();
            grid.at(i+1, j+1) = particle;
        }
        else if(i > 0 && j < COLUMNS-1 && !grid.is_cell_empty(curr_cell.up_left()) && grid.at(i-1, j+1)->is_affected_by("fire")) {
            delete grid.at(i-1, j+1);
            Particle* particle = new FireParticle();
            grid.at(i-1, j+1) = particle;
        }
    }
}

bool FireParticle::is_affected_by(const std::string particle) const {
    return false;
}

Color3 FireParticle::get_color() const {
    if(lifetime_left_ <= 5)
        return Color3(1.0f, 0.6f, 0.0f);
    return Color3(1.00f, 0.0f, 0.0f); 
}

void FireParticle::set_spawn_on_death(Particle* particle) {
    spawn_on_death_ = particle;
}

//------------------------------
// Steam Particle
//------------------------------
const std::string SteamParticle::name = "Steam";

void SteamParticle::update(const int i, const int j, Grid& grid) {
    Cell curr_cell(i, j);
    lifetime_left_--;

    if(lifetime_left_ <= 0) {
        delete grid.at(curr_cell.x, curr_cell.y);
        grid.at(curr_cell.x, curr_cell.y) = NULL;
        return;
    }

    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib_100(1,100);
    int scatter_chance = distrib_100(rand_generator);

    if(j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up()) && will_go_up(scatter_chance)) {
        grid.swap(curr_cell, curr_cell.up());
    }
    else if(i > 0 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_left()) && will_go_up_left(scatter_chance)) {
        grid.swap(curr_cell, curr_cell.up_left());
    }
    else if(i < ROWS-1 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_right()) && will_go_up_right(scatter_chance)) {
        grid.swap(curr_cell, curr_cell.up_right());
    }
    else if(i > 0 && grid.is_cell_empty(curr_cell.left()) && scatter_chance <= 50) {
        grid.swap(curr_cell, curr_cell.left());
    }
    else if(i < ROWS-1 && grid.is_cell_empty(curr_cell.right()) && scatter_chance > 50) {
        grid.swap(curr_cell, curr_cell.right());
    }
}

bool SteamParticle::is_affected_by(const std::string particle) const {
    return false;
}

Color3 SteamParticle::get_color() const {
    return Color3(0.75f, 0.75f, 0.75f); 
}

bool SteamParticle::will_go_up(int val) const {
    if(val <= up_chance) {
        return true;
    }
    return false;
}

bool SteamParticle::will_go_up_left(int val) const {
    if(val > up_chance && val <= up_chance + up_left_chance) {
        return true;
    }
    return false;
}

bool SteamParticle::will_go_up_right(int val) const {
    if(val > up_chance + up_left_chance) {
        return true;
    }
    return false;
}
