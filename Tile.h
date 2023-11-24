#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef MINESWEEP_TILE_H
#define MINESWEEP_TILE_H

struct Tile {
    bool is_mine;
    bool is_revealed;
    bool is_pressed;
    int adjacent_mines;
    vector<Tile*> adjacent_tiles;
    sf::Sprite unrevealed;
    sf::Sprite revealed;

    /* Tile Functionality */
    Tile() {

    }

    void reveal() {

    }

    void toggleFlag() {

    }


};





#endif //MINESWEEP_TILE_H
