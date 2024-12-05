#include "particle.hpp"
#include "particle_types.hpp"
#include "random.hpp"


//------------------------------
// Sand Particle
//------------------------------
const std::string SandParticle::name = "Sand";

void SandParticle::update(const int i, const int j, Grid& grid) {
    Cell cell(i, j);

    if(j > 0 && grid.is_cell_empty(cell.down())) {
        grid.swap(cell, cell.down());
    }
    else if(j > 0 && i > 0 && grid.is_cell_empty(cell.down_left())) {
        grid.swap(cell, cell.down_left());
    }
    else if(j > 0 && i < COLUMNS-1 && grid.is_cell_empty(cell.down_right())) {
        grid.swap(cell, cell.down_right());
    }

    constexpr int MOVE_LEFT = 0;
    const int MOVEMENT_DIRECTION = gen_random_num(0, 1);

    if(MOVEMENT_DIRECTION == MOVE_LEFT) {
        if(j > 0 && i > 0 && !grid.is_cell_empty(cell.down_left()) && grid.at(cell.down_left())->is_affected_by(ParticleType::SAND)) {
            grid.swap(cell, cell.down_left());
        }
    }
    // Move right.
    else {
        if(j > 0 && i < COLUMNS-1 && !grid.is_cell_empty(cell.down_right()) && grid.at(cell.down_right())->is_affected_by(ParticleType::SAND)) {
            grid.swap(cell, cell.down_right());
        }
    }
}

bool SandParticle::is_affected_by(const int particle_id) const {
    return false;
}

void SandParticle::interact_with(const int particle_id, Cell cell, Grid& grid) const {
    switch(particle_id) {
        default: {
        }
    }
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

    constexpr int MOVE_RIGHT = 1;
    const int MOVE_DIRECTION = gen_random_num(0, MOVE_RIGHT);

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
    else if(i-get_dispersion_rate() > 0 && MOVE_DIRECTION != MOVE_RIGHT) {
        grid.move_cell_left_until_blocked(Cell(i, j), get_dispersion_rate());
    }
    // Move particle right if nothing is there
    else if(i+get_dispersion_rate() < ROWS && MOVE_DIRECTION == MOVE_RIGHT) {
        grid.move_cell_right_until_blocked(Cell(i, j), get_dispersion_rate());
    }
}

bool WaterParticle::is_affected_by(const int particle_id) const {
    switch(particle_id) {
        case ParticleType::FIRE: {
            return true;
        }
        // Temporary until I figure out how
        // to distinguish the difference between a particle interaction
        // that affects movement vs something that is a conversion.
        case ParticleType::SAND: {
            return true;
        }
        default: {
            return false;
        }
    }
}

void WaterParticle::interact_with(const int particle_id, Cell cell, Grid& grid) const {
    switch(particle_id) {
        case ParticleType::FIRE: {
            delete grid.at(cell);
            Particle* particle = new SteamParticle();
            grid.at(cell) = particle;
            break;
        }
        default: {

        }
    }
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

bool WallParticle::is_affected_by(const int particle_id) const {
    return false;
}

void WallParticle::interact_with(const int particle_id, Cell cell, Grid& grid) const {
    switch(particle_id) {
        default: {

        }
    }
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
    m_lifetime_left--;

    if(m_lifetime_left <= 0) {
        grid.remove(curr_cell.x, curr_cell.y);
        return;
    }

    const Direction direction = gen_random_weighted_vertical_direction(80, 10, 10);

    if(j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up()) && direction == Direction::UP) {
        grid.swap(curr_cell, curr_cell.up());
    }
    else if(i > 0 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_left()) && direction == Direction::UP_LEFT) {
        grid.swap(curr_cell, curr_cell.up_left());
    }
    else if(i < ROWS-1 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_right()) && direction == Direction::UP_RIGHT) {
        grid.swap(curr_cell, curr_cell.up_right());
    }
    else if(i > 0 && grid.is_cell_empty(curr_cell.left())) {
        grid.swap(curr_cell, curr_cell.left());
    }
    else if(i < ROWS-1 && grid.is_cell_empty(curr_cell.right())) {
        grid.swap(curr_cell, curr_cell.right());
    }
}

bool SmokeParticle::is_affected_by(const int particle_id) const {
    return false;
}

void SmokeParticle::interact_with(const int particle_id, Cell cell, Grid& grid) const {
    switch(particle_id) {
        default: {

        }
    }
}

Color3 SmokeParticle::get_color() const {
    return Color3(0.4f, 0.4f, 0.4f); 
}

//------------------------------
// Wood Particle
//------------------------------
const std::string WoodParticle::name = "Wood";

void WoodParticle::update(const int i, const int j, Grid& grid) {
    // The WoodParticle doesn't move.
}

bool WoodParticle::is_affected_by(const int particle_id) const {
    switch(particle_id) {
        case ParticleType::FIRE: {
            return true;
        }
        default: {
            return false;
        }
    }
}

void WoodParticle::interact_with(const int particle_id, Cell cell, Grid& grid) const {
    switch(particle_id) {
        case ParticleType::FIRE: {
            delete grid.at(cell);
            Particle* particle = new FireParticle();
            grid.at(cell) = particle;

            if(grid.is_cell_empty(cell.up())) {
                grid.at(cell.up()) = new SmokeParticle();
            }
            break;
        }
        default: {

        }
    }
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

    if(lifetime_left_ <= 0) {
        delete grid.at(i, j);
        grid.at(i, j) = NULL;
        return;
    }

    int flame_expansion_chance = gen_random_num(1, 100);
    constexpr int THRESHOLD = 90;

    if(flame_expansion_chance > THRESHOLD) {
        if(j < COLUMNS-1 && i < ROWS-1 && grid.is_cell_empty(curr_cell.up_right())) {
            grid.at(i + 1, j + 1) = new FireParticle();
        }
        else if(j < COLUMNS-1 && i > 0 && grid.is_cell_empty(curr_cell.up_left())) {
            grid.at(i - 1, j + 1) = new FireParticle();
        }
    }

    ignite_surroundings(curr_cell, grid);
}

bool FireParticle::is_affected_by(const int particle_id) const {
    return false;
}

void FireParticle::interact_with(const int particle_id, Cell cell, Grid& grid) const {
    switch(particle_id) {
        default: {

        }
    }
}

Color3 FireParticle::get_color() const {
    if(lifetime_left_ <= 5)
        return Color3(1.0f, 0.6f, 0.0f);
    return Color3(1.00f, 0.0f, 0.0f); 
}

void FireParticle::set_spawn_on_death(Particle* particle) {
    spawn_on_death_ = particle;
}

void FireParticle::ignite_surroundings(const Cell cell, Grid& grid) {
    delay_until_inflamed_--;

    if(delay_until_inflamed_ <= 0) {
        delay_until_inflamed_ = 5;

        if(cell.y < COLUMNS-1 && !grid.is_cell_empty(cell.up()) && grid.at(cell.up())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.up())->interact_with(ParticleType::FIRE, cell.up(), grid);
        }
        else if(cell.y > 0 && !grid.is_cell_empty(cell.down()) && grid.at(cell.down())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.down())->interact_with(ParticleType::FIRE, cell.down(), grid);
        }
        else if(cell.x > 0 && !grid.is_cell_empty(cell.left()) && grid.at(cell.left())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.left())->interact_with(ParticleType::FIRE, cell.left(), grid);
        }
        else if(cell.x < ROWS-1 && !grid.is_cell_empty(cell.right()) && grid.at(cell.right())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.right())->interact_with(ParticleType::FIRE, cell.right(), grid);
        }
        else if(cell.x > 0 && cell.y > 0 && !grid.is_cell_empty(cell.down_left()) && grid.at(cell.down_left())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.down_left())->interact_with(ParticleType::FIRE, cell.down_left(), grid);
        }
        else if(cell.x < ROWS-1 && cell.y > 0 && !grid.is_cell_empty(cell.down_right()) && grid.at(cell.down_right())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.down_right())->interact_with(ParticleType::FIRE, cell.down_right(), grid);
        }
        else if(cell.x < ROWS-1 && cell.y < COLUMNS-1 && !grid.is_cell_empty(cell.up_right()) && grid.at(cell.up_right())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.up_right())->interact_with(ParticleType::FIRE, cell.up_right(), grid);
        }
        else if(cell.x > 0 && cell.y < COLUMNS-1 && !grid.is_cell_empty(cell.up_left()) && grid.at(cell.up_left())->is_affected_by(ParticleType::FIRE)) {
            grid.at(cell.up_left())->interact_with(ParticleType::FIRE, cell.up_left(), grid);
        }
    }
}

//------------------------------
// Steam Particle
//------------------------------
const std::string SteamParticle::name = "Steam";

void SteamParticle::update(const int i, const int j, Grid& grid) {
    Cell curr_cell(i, j);
    m_lifetime_left--;

    if(m_lifetime_left <= 0) {
        grid.remove(curr_cell.x, curr_cell.y);
        return;
    }

    Direction direction = gen_random_weighted_vertical_direction(60, 20, 20);

    if(j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up()) && direction == Direction::UP) {
        grid.swap(curr_cell, curr_cell.up());
    }
    else if(i > 0 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_left()) && direction == Direction::UP_LEFT) {
        grid.swap(curr_cell, curr_cell.up_left());
    }
    else if(i < ROWS-1 && j < COLUMNS-1 && grid.is_cell_empty(curr_cell.up_right()) && direction == Direction::UP_RIGHT) {
        grid.swap(curr_cell, curr_cell.up_right());
    }
    else if(i > 0 && grid.is_cell_empty(curr_cell.left())) {
        grid.swap(curr_cell, curr_cell.left());
    }
    else if(i < ROWS-1 && grid.is_cell_empty(curr_cell.right())) {
        grid.swap(curr_cell, curr_cell.right());
    }
}

bool SteamParticle::is_affected_by(const int particle_id) const {
    return false;
}

void SteamParticle::interact_with(const int particle_id, Cell cell, Grid& grid) const {
    switch(particle_id) {
        default: {

        }
    }
}

Color3 SteamParticle::get_color() const {
    return Color3(0.75f, 0.75f, 0.75f); 
}

//------------------------------
// Gas
//------------------------------
