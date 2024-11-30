#include <csignal>
#include <iostream>

#include "grid.hpp"
#include "particle.hpp"

Cell Cell::left() const {
    return Cell(x - 1, y);
}
Cell Cell::right() const {
    return Cell(x + 1, y);
}
Cell Cell::up()  const{
    return Cell(x, y + 1);
}
Cell Cell::up_left() const {
    return Cell(x - 1, y + 1);
}
Cell Cell::up_right() const {
    return Cell(x + 1, y + 1);
}
Cell Cell::down() const {
    return Cell(x, y - 1);
}
Cell Cell::down_right() const {
    return Cell(x + 1, y - 1);
}
Cell Cell::down_left() const {
    return Cell(x - 1, y - 1);
}

Grid::Grid() {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            grid[i][j] = NULL;
        }
    }
}

Grid::~Grid() {
    clear();
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

Particle* & Grid::at(const Cell cell) {
    try{
        if((cell.x >= 0 && cell.y >= 0) && (cell.x < ROWS && cell.y < COLUMNS)) {
            return grid[cell.x][cell.y];
        }
        else {
            throw -1;
        }
    }
    catch(int error) {
        std::cerr << "Index out of bounds: " << cell.x << ' ' << cell.y << '\n';
        throw error;
    }
}

void Grid::insert(const int x, const int y, Particle* particle) {
    if(is_within_bounds(x, y) && is_cell_empty(x, y))
        grid[x][y] = particle;
    /*
    else if(!is_cell_empty(x, y))
        std::cerr << "Warn: insert called when the cell is not empty: " 
                  << x << ' ' << y << '\n';
    */
}

void Grid::insert(const Cell cell, Particle* particle) {
    if(is_within_bounds(cell.x, cell.y) && is_cell_empty(cell.x, cell.y))
        grid[cell.x][cell.y] = particle;
    /*
    else if(!is_cell_empty(cell.x, cell.y))
        std::cerr << "Warn: insert called when the cell is not empty: " 
                  << cell.x << ' ' << cell.y << '\n';
    */
}

void Grid::remove(const int x, const int y) {
    if(is_within_bounds(x, y) && !is_cell_empty(x, y)) {
        delete grid[x][y];
        grid[x][y] = NULL;
    }
    else if(!is_cell_empty(x, y))
        std::cerr << "Warn: remove called when the cell is empty: " 
                  << x << ' ' << y << '\n';
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

void Grid::reset_has_been_drawn_flags() {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            if(!is_cell_empty(i, j))
                at(i, j)->has_been_drawn = false;
        }
    }
}

void Grid::clear() {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            if(grid[i][j] != NULL) {
                delete grid[i][j];
                grid[i][j] = NULL;
            }
        }
    }
}

bool Grid::is_within_bounds(const int x, const int y) {
    if(x >= 0 && y >= 0 && x < ROWS && y < COLUMNS)
        return true;
    else {
        std::cerr << "Warn: indices out of range: " << x << ' ' << y << '\n';
        return false;
    }
}

//------------
//  Utility Functions
//------------
glm::vec3 grid_to_ndc(int i, int j, const int width, const int height) {
    glm::vec3 point;
    point.x = (((float)i/width)*2)-1;
    point.y = 1.0*((((float)j/height)*2)-1);
    point.z = 0.0;
    return point;
}
