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
    bool is_pressed;
    int adjacent_mines;
    vector<Tile*> adjacent_tiles;
    sf::Texture unrevealed_text;
    sf::Texture revealed_text;
    sf::Sprite state;


    /* Tile Functionality */
    Tile() {
        unrevealed_text.loadFromFile("files/images/tile_hidden.png");
        revealed_text.loadFromFile("files/images/tile_revealed.png");
        state.setTexture(unrevealed_text);
    }

    void reveal() {
        state.setTexture(revealed_text);
    }

    void toggleFlag() {

    }


};





#endif //MINESWEEP_TILE_H
