// This module contains Class definitions for the
// Particle abstract base class and its derived classes.

#ifndef PARTICLE_H
#define PARTICLE_H

#include <cstddef>
#include <glm/vec3.hpp>

// This Abstract Class provides the base functionality a Particle should have.
// The primary use case of this interface is to use it in places where
// you would like to leverage polymorphism.
class Particle {
public:
    Particle() {}

    // Updates the location of the particle based on its movement rules
    virtual void update(int xpos, int ypos, Particle* (&grid)[550][550]) const = 0;

    virtual glm::vec3 get_color() const;

    bool has_been_drawn = false;
};

class SandParticle: public Particle {
public:
    SandParticle() {};

    void update(const int i, const int j, Particle* (&grid)[550][550]) const override; 

    glm::vec3 get_color() const override;
};

class WaterParticle: public Particle {
public:
    WaterParticle() {};

    void update(int i, int j, Particle* (&grid)[550][550]) const override; 

    glm::vec3 get_color() const override;
};

#endif
