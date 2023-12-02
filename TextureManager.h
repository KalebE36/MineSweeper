#pragma once
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_TEXTUREMANAGER_H
#define MINESWEEP_TEXTUREMANAGER_H


using namespace std;
class TextureManager {
    static unordered_map<string, sf::Texture> textures;
public:
    static sf::Texture& getTexture(string textureName);
};


#endif //MINESWEEP_TEXTUREMANAGER_H
