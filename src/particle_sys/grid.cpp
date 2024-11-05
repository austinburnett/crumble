#include <iostream>

#include "grid.hpp"

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

int Grid::count() {
    int count = 0;

    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            if(grid[i][j] != NULL)
                count++;
        }
    }
    return count;
}
