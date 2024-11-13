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
    virtual void update(const int i, const int j, Grid& grid) const = 0; 

    // Returns the color of the particle, composed of three channels- RGB.
    virtual glm::vec3 get_color() const = 0;

public:
    bool has_been_drawn = false;   // Has this particle been updated.

    bool is_flammable   = false;   // This will catch fire.
    bool is_erodible    = false;   // This will react to acid.
};

class Liquid {
public:
    int dispersion_rate_;         // How fast the particle disperses.
    float density;                // Determines if a particle will float or sink.
};

class Solid {

};

class Gas {

};

class Plasma {

};

class SandParticle: public Particle, Solid {
public:
    SandParticle() = default;

    void update(const int i, const int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 0;
    const static std::string name;
};

class WaterParticle: public Particle, Liquid {
public:
    WaterParticle() = default;

    void update(const int i, const int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 1;
    const static std::string name;
};

class WallParticle: public Particle, Solid {
public:
    WallParticle() = default;

    void update(const int i, const int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 2;
    const static std::string name;
};

class SmokeParticle: public Particle, Gas {
public:
    SmokeParticle() = default;

    void update(const int i, const int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 3;
    const static std::string name;
};

class WoodParticle: public Particle, Solid {
public:
    WoodParticle() = default;

    void update(const int i, const int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 4;
    const static std::string name;

    bool is_flammable = true;
    bool is_erodible  = true;
};

class FireParticle: public Particle, Plasma {
public:
    FireParticle() = default;

    void update(const int i, const int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

public:
    // Both static members must be present on every Particle type.
    constexpr static int id = 5;
    const static std::string name;
};

void reset_has_been_drawn_flags(Grid &grid);

#endif
