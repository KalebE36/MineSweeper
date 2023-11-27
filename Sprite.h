#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <string>
#include "TextureManager.h"
#ifndef MINESWEEP_SPRITE_H
#define MINESWEEP_SPRITE_H

using namespace std;

struct Sprite {
    sf::Texture sprite_texture;
    sf::Sprite new_sprite;
    int num_cols, num_rows;
    float x, y;

    Sprite() {}

    Sprite(sf::Texture& sprite_texture, int num_cols, int num_rows, float x, float y) {
        new_sprite.setTexture(sprite_texture);
        new_sprite.setPosition(x, y);
    }

};




#endif //MINESWEEP_SPRITE_H
