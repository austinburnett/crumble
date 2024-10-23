#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include "grid.hpp"
#include <glm/vec3.hpp>


// This Abstract Class provides the base functionality a Particle should have.
// The primary use case of this interface is to use it in places where
// you would like to leverage polymorphism.
class Particle {
public:
    Particle() = default;

    // Updates the location of the particle based on its movement rules
    virtual void update(int xpos, int ypos, Grid& grid) const = 0;

    virtual glm::vec3 get_color() const;

    bool has_been_drawn = false;
};

class SandParticle: public Particle {
public:
    SandParticle() = default;

    void update(const int i, const int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

    constexpr static int id = 0;
    const static std::string name;
};

class WaterParticle: public Particle {
public:
    WaterParticle() = default;

    void update(int i, int j, Grid& grid) const override; 

    glm::vec3 get_color() const override;

    constexpr static int id = 1;
    const static std::string name;
};

void reset_has_been_drawn_flags(Grid &grid);

#endif
