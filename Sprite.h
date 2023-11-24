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
    string texture_name;
    int num_cols, num_rows;
    float x, y;

    Sprite() {}

    Sprite(string texture_name, int num_cols, int num_rows, float x, float y) {
        this->texture_name = texture_name;
        sprite_texture.loadFromFile(texture_name);
        new_sprite.setTexture(sprite_texture);
        new_sprite.setOrigin(sprite_texture.getSize().x/2.0f,sprite_texture.getSize().y/2.0f);
        new_sprite.setPosition(x, y);
    }

};




#endif //MINESWEEP_SPRITE_H
