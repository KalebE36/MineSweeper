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
    bool is_flagged;
    int adjacent_mines;
    int tile_num;
    vector<Tile*> adjacent_tiles;
    sf::Sprite state;
    sf::Sprite mine_sprite;
    sf::Sprite flag_sprite;


    /* Tile Functionality */
    Tile() {}

    Tile(sf::Texture& default_texture, sf::Texture& mine_texture, sf::Texture& flag_texture, int tile_num) {
        state.setTexture(default_texture);
        mine_sprite.setTexture(mine_texture);
        flag_sprite.setTexture(flag_texture);
        this->tile_num = tile_num;
        is_mine = false;
        is_revealed = false;
        is_flagged = false;

    }

    void updateRevealedTile(sf::Texture& revealed_texture) {
        is_revealed = true;
        state.setTexture(revealed_texture);
    }


    void reveal() {
    }

    void toggleFlag() {
        is_flagged = true;
    }


};





#endif //MINESWEEP_TILE_H
