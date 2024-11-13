#ifndef GRID_H
#define GRID_H

// Forward declare the class so a Particle
// data member can be declared.
class Particle;

// Settings
static const unsigned int ROWS = 550;
static const unsigned int COLUMNS = 550;

class Grid {
private:
    // [0][0] is the bottom-left corner of the window
    Particle* grid[ROWS][COLUMNS];

public:
    Grid();

    Grid(const Grid& other)           = delete;

    Grid(Grid&& other)                = delete;

    Grid operator=(const Grid& other) = delete;

    Grid operator=(Grid&& other)      = delete;

    // Returns the Particle at the cell specified.
    Particle* & at(const int i, const int j);

    // Returns the number of cells filled with particles.
    int count() const;

    bool is_cell_empty(const int i, const int j) const;

    // Swaps the values in both positions specified.
    void swap(const int i1, const int j1, const int i2, const int j2);
};

// This should be called once per frame to ensure
// the particles are only updated once per frame.
void reset_has_been_drawn_flags(Grid& grid);

#endif
