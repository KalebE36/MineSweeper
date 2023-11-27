#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Sprite.h"

using namespace std;

#ifndef MINESWEEP_TILE_H
#define MINESWEEP_TILE_H

struct Tile {
    bool is_mine;
    bool is_revealed;
    int adjacent_mines;
    int tile_num;
    vector<Tile*> adjacent_tiles;
    sf::Sprite state;


    /* Tile Functionality */
    Tile() {}

    Tile(sf::Texture& default_texture, int tile_num) {
        state.setTexture(default_texture);
        this->tile_num = tile_num;
        is_mine = false;

    }

    void updateRevealedTile(sf::Texture& revealed_texture) {
        state.setTexture(revealed_texture);
    }

    void updateIfMine() {

    }


    void reveal() {
    }

    void toggleFlag() {

    }


};





#endif //MINESWEEP_TILE_H
