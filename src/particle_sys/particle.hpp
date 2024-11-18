#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include <glm/vec3.hpp>

#include "grid.hpp"

// Clarify its usage.
using Color3 = glm::vec3;

// This stores data and functionality present in all four
// states of matter: Liquid, Solid, Gas, and Plasma.
class Particle {
public:
    Particle() = default;

    // Determines the behavior of the particle in the simulation.
    virtual void update(const int i, const int j, Grid& grid) = 0; 

    // Returns the color of the particle, composed of three channels- RGB.
    virtual Color3 get_color() const = 0;

    // Determines whether or not this matter will catch fire.
    virtual bool is_flammable() const = 0;

public:
    bool has_been_drawn  = false;
};

class Liquid {
public:
    // Return the rate at which it spreads out horizontally.
    virtual int get_dispersion_rate() const = 0;
};

class Solid {
};

class Gas {
    // They need to move rapidly in all directions.
    // Return the rate at which it spreads out horizontally and vertically.
    //virtual int get_expansion_rate() const = 0;
};

class Plasma {
public:
};

class SandParticle: public Particle, Solid {
public:
    SandParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    Color3 get_color() const override;

    bool is_flammable() const override;

public:
    constexpr static int id = 0;
    const static std::string name;
};

class WaterParticle: public Particle, public Liquid {
public:
    WaterParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    Color3 get_color() const override;

    bool is_flammable() const override;

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

    bool is_flammable() const override;

public:
    constexpr static int id = 2;
    const static std::string name;
};

class SmokeParticle: public Particle, Gas {
public:
    SmokeParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    
    Color3 get_color() const override;

    bool is_flammable() const override;

public:
    constexpr static int id = 3;
    const static std::string name;
};

class WoodParticle: public Particle, public Solid {
public:
    WoodParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    Color3 get_color() const override;

    bool is_flammable() const override;

public:
    constexpr static int id = 4;
    const static std::string name;
};

class FireParticle: public Particle, public Plasma {
public:
    FireParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    Color3 get_color() const override;

    bool is_flammable() const override;

public:
    constexpr static int id = 5;
    const static std::string name;
    int lifetime_left_          = 10;      // The duration of the fire.
    int delay_until_inflamed_   = 5;       // Slows the spread of fire.
};

#endif
