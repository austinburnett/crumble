#pragma once

#include <glm/vec3.hpp>

// Forward declare the class so a Particle
// data member can be declared.
class Particle;

// Settings
inline const unsigned int ROWS    = 550;
inline const unsigned int COLUMNS = 550;

struct Cell {
public:
    Cell(int x, int y): x(x), y(y) {}
    Cell left()       const;
    Cell right()      const;
    Cell up()         const;
    Cell up_right()   const;
    Cell up_left()    const;
    Cell down()       const;
    Cell down_right() const;
    Cell down_left()  const;

public:
    int x, y;
};


class Grid {
private:
    // [0][0] is the bottom-left corner of the window
    Particle* grid[ROWS][COLUMNS];

private:
    bool is_within_bounds(const int x, const int y);

public:
    Grid();
    ~Grid();
    Grid(const Grid& other)           = delete;
    Grid(Grid&& other)                = delete;
    Grid operator=(const Grid& other) = delete;
    Grid operator=(Grid&& other)      = delete;

    // Returns the item at the position specified.
    Particle* & at(const int i, const int j);
    Particle* & at(const Cell cell);

    void insert(const int x, const int y, Particle* particle);
    void insert(const Cell cell, Particle* particle);
    void remove(const int x, const int y);

    // Returns the number of items in the grid.
    int count() const;

    bool is_cell_empty(const int i, const int j) const;
    bool is_cell_empty(Cell cell);

    // Moves the cell to the destination or as close
    // as possible if there are any filled cells inbetween.
    // Returns the new location of the cell that is moved.
    void move_cell_left_until_blocked(Cell cell, const int times);

    // Moves the cell to the destination or as close
    // as possible if there are any filled cells inbetween.
    // Returns the new location of the cell that is moved.
    void move_cell_right_until_blocked(Cell cell, const int times);

    // Swaps the values in both positions specified.
    void swap(const int i1, const int j1, const int i2, const int j2);

    // Swaps the values in both cells specified.
    void swap(const Cell cell1, const Cell cell2);

    // This should be called once per frame to ensure
    // the particles are only updated once per frame.
    void reset_has_been_drawn_flags();

    // Frees all the stored pointers.
    // This can "clear" the matter from the window.
    void clear();
};

//-------------------
// Utility Functions
//-------------------

// Convert from the grid with the ranges [0, ROWS] and [0, COLUMNS] to ndc.
// Opengl expects vertices between [-1, 1] and a y-axis pointing up.
glm::vec3 grid_to_ndc(int i, int j, const int width, const int height);
