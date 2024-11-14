#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include <glm/vec3.hpp>

#include "grid.hpp"


// This stores data and functionality present in all four
// states of matter: Liquid, Solid, and Gas, and Fire.
class Particle {
public:
    Particle() = default;

    // Updates the location of the particle based on its movement rules.
    virtual void update(const int i, const int j, Grid& grid) = 0; 

    // Returns the color of the particle, composed of three channels- RGB.
    virtual glm::vec3 get_color() const = 0;

    // Determines whether or not this matter will catch fire.
    virtual bool is_flammable() const = 0;

public:
    bool has_been_drawn  = false;   // Has this this been updated.
};

class Liquid {
public:
    int dispersion_rate_;           // How fast this spreads out.
};

class Solid {

};

class Gas {

};

class Plasma {
public:
};

class SandParticle: public Particle, Solid {
public:
    SandParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    glm::vec3 get_color() const override;

    bool is_flammable() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 0;
    const static std::string name;
};

class WaterParticle: public Particle, Liquid {
public:
    WaterParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    glm::vec3 get_color() const override;

    bool is_flammable() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 1;
    const static std::string name;
};

class WallParticle: public Particle, Solid {
public:
    WallParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    
    glm::vec3 get_color() const override;

    bool is_flammable() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 2;
    const static std::string name;
};

class SmokeParticle: public Particle, Gas {
public:
    SmokeParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 
    
    glm::vec3 get_color() const override;

    bool is_flammable() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 3;
    const static std::string name;
};

class WoodParticle: public Particle, public Solid {
public:
    WoodParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    glm::vec3 get_color() const override;

    bool is_flammable() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 4;
    const static std::string name;
};

class FireParticle: public Particle, Plasma {
public:
    FireParticle() = default;

    void update(const int i, const int j, Grid& grid) override; 

    glm::vec3 get_color() const override;

    bool is_flammable() const override;

    void spawn_on_death();

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 5;
    const static std::string name;
    int lifetime_        = 50;
    int frames_to_wait   = 10;
};

#endif
