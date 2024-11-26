#pragma once

#include <string>

#include <glm/vec3.hpp>

#include "grid.hpp"

using Color3 = glm::vec3;


class Particle {
public:
    Particle() = default;
    virtual ~Particle() = default;

    // Determines the behavior of the particle in the simulation.
    virtual void update(const int i, const int j, Grid& grid) = 0; 

    // Used to lookup what this particle is affected by.
    // Is there a reaction between these two particles.
    virtual bool is_affected_by(const int particle_id) const = 0;

    // Determines how the particle interacts with the one specified.
    // How this particle with react to the particle specified.
    virtual void interact_with(const int particle_id, Cell cell, Grid& grid) const = 0;

    // Returns the color in RGB format.
    virtual Color3 get_color() const = 0;

public:
    bool has_been_drawn  = false;
};

class Liquid {
protected:
    // Return the rate at which it spreads out horizontally.
    virtual int get_dispersion_rate() const = 0;
};

class Solid {
protected:
};

// They need to move rapidly in all directions.
// Return the rate at which it spreads out horizontally and vertically.
class Gas {
public:
    Gas() = default;
    virtual ~Gas() = default;

protected:
    /*
    virtual int get_vertical_scatter_rate() const = 0;
    virtual int get_horizontal_scatter_rate() const = 0;
    */

    virtual bool will_go_up_left(int val) const = 0;
    virtual bool will_go_up_right(int val) const = 0;
    virtual bool will_go_up(int val) const = 0;

protected:
    // The sum of chances must be 100.
    int up_chance = 80;
    int up_left_chance = 10;
    int up_right_chance = 10;
};

class Plasma {
protected:
};

class SandParticle: public Particle, Solid {
public:
    SandParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    Color3 get_color() const override;
    bool is_affected_by(const int particle_id) const override;
    void interact_with(const int particle_id, Cell cell, Grid& grid) const override;

public:
    constexpr static int id = 0;
    const static std::string name;
};

class WaterParticle: public Particle, public Liquid {
public:
    WaterParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    Color3 get_color() const override;
    bool is_affected_by(const int particle_id) const override;
    void interact_with(const int particle_id, Cell cell, Grid& grid) const override;

    int get_dispersion_rate() const override;

public:
    constexpr static int id = 1;
    const static std::string name;
};

class WallParticle: public Particle, Solid {
public:
    WallParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    Color3 get_color() const override;
    bool is_affected_by(const int particle_id) const override;
    void interact_with(const int particle_id, Cell cell, Grid& grid) const override;

public:
    constexpr static int id = 2;
    const static std::string name;
};

class SmokeParticle: public Particle, Gas {
public:
    SmokeParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    Color3 get_color() const override;
    bool is_affected_by(const int particle_id) const override;
    void interact_with(const int particle_id, Cell cell, Grid& grid) const override;

    /*
    int get_vertical_scatter_rate() const override;
    int get_horizontal_scatter_rate() const override;
    */
    bool will_go_up_left(int val) const override;
    bool will_go_up_right(int val) const override;
    bool will_go_up(int val) const override;

public:
    constexpr static int id = 3;
    const static std::string name;
    int lifetime_left_          = 2000;      // The duration.

protected:
    // Modify from top to bottom.
    int up_chance       = 40;
    int up_left_chance  = 30;
    int up_right_chance = 30;
};

class WoodParticle: public Particle, public Solid {
public:
    WoodParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    Color3 get_color() const override;
    bool is_affected_by(const int particle_id) const override;
    void interact_with(const int particle_id, Cell cell, Grid& grid) const override;

    Particle* by_fire() const;

public:
    constexpr static int id = 4;
    const static std::string name;
};

class FireParticle: public Particle, public Plasma {
public:
    FireParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    Color3 get_color() const override;
    bool is_affected_by(const int particle_id) const override;
    void interact_with(const int particle_id, Cell cell, Grid& grid) const override;

public:
    constexpr static int id = 5;
    const static std::string name;

private:
    void set_spawn_on_death(Particle* particle);
    void ignite_surroundings(const Cell cell, Grid& grid);

private:
    Particle* spawn_on_death_ = NULL;
    int delay_until_inflamed_   = 5;       // Slows the spread of fire.
    int lifetime_left_          = 10;      // The duration of the fire.
};

class SteamParticle: public Particle, Gas {
public:
    SteamParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    Color3 get_color() const override;
    bool is_affected_by(const int particle_id) const override;
    void interact_with(const int particle_id, Cell cell, Grid& grid) const override;

    /*
    int get_vertical_scatter_rate() const override;
    int get_horizontal_scatter_rate() const override;
    */
    bool will_go_up_left(int val) const override;
    bool will_go_up_right(int val) const override;
    bool will_go_up(int val) const override;

public:
    constexpr static int id = 6;
    const static std::string name;
    int lifetime_left_          = 2000;      // The duration.

protected:
    // Modify from top to bottom.
    int up_chance       = 40;
    int up_left_chance  = 30;
    int up_right_chance = 30;
};
