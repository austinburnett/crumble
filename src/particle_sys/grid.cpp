#include <csignal>
#include <iostream>

#include "grid.hpp"
#include "particle.hpp"

Grid::Grid() {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            grid[i][j] = NULL;
        }
    }
}

Particle* & Grid::at(const int i, const int j) {
    try{
        if((i >= 0 && j >= 0) && (i < ROWS && j < COLUMNS)) {
            return grid[i][j];
        }
        else {
            throw -1;
        }
    }
    catch(int error) {
        std::cerr << "Index out of bounds: " << i << ' ' << j << '\n';
        throw error;
    }
}

int Grid::count() const {
    int count = 0;

    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            if(grid[i][j] != NULL)
                count++;
        }
    }
    return count;
}

bool Grid::is_cell_empty(const int i, const int j) const {
    if(grid[i][j] == NULL) {
        return true;
    }
    return false;
}

bool Grid::is_cell_empty(Cell cell) {
    if(grid[cell.x][cell.y] == NULL) {
        return true;
    }
    return false;
}

void Grid::swap(const int i1, const int j1, const int i2, const int j2) {
    Particle* temp = this->at(i2, j2);
    this->at(i2, j2) = this->at(i1, j1);
    this->at(i1, j1) = temp;
}

void Grid::swap(const Cell cell1, const Cell cell2) {
    Particle* temp = this->at(cell2.x, cell2.y);
    this->at(cell2.x, cell2.y) = this->at(cell1.x, cell1.y);
    this->at(cell1.x, cell1.y) = temp;
}

void Grid::move_cell_left_until_blocked(Cell cell, int times) {
    Cell furthest_empty_cell = cell;

    for(int i = 1; i <= times; ++i) {
        if(!is_cell_empty(cell.x - i, cell.y)) {
            break;
        }
        else if(is_cell_empty(cell.x - i, cell.y)) {
            furthest_empty_cell.x = cell.x - i;
        }
    }

    if(furthest_empty_cell.x != cell.x) {
        swap(cell, furthest_empty_cell);
    }
}

void Grid::move_cell_right_until_blocked(Cell cell, int times) {
    Cell furthest_empty_cell = cell;

    for(int i = 1; i <= times; ++i) {
        if(!is_cell_empty(cell.x + i, cell.y)) {
            break;
        }
        else if(is_cell_empty(cell.x + i, cell.y)) {
            furthest_empty_cell.x = cell.x + i;
        }
    }

    if(furthest_empty_cell.x != cell.x) {
        swap(cell, furthest_empty_cell);
    }
}

//---------------------
// Utilities
//---------------------
void reset_has_been_drawn_flags(Grid &grid) {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            if(!grid.is_cell_empty(i, j))
                grid.at(i, j)->has_been_drawn = false;
        }
    }
}
