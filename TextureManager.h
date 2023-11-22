#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_TEXTUREMANAGER_H
#define MINESWEEP_TEXTUREMANAGER_H


using namespace std;
class TextureManager {
    static unordered_map<string, sf::Texture> textures;
public:
    static sf::Texture& getTexture(string textureName) {
        auto result = textures.find(textureName);
        if(result == textures.end()) {
            // Texture does not already exist in the map, go get it!
            sf::Texture newTexture;
            newTexture.loadFromFile("images/" + textureName + ".png"); textures[textureName] = newTexture;
            return textures[textureName];
        } else {
            // Texture already exists, return it!
            return result->second;
        }
    }
};


#endif //MINESWEEP_TEXTUREMANAGER_H
