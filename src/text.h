#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_TEXT_H
#define MINESWEEP_TEXT_H
using namespace std;



struct Text {
    sf::Text new_text;
    sf::Font font;

    Text() {
        font.loadFromFile("../font.ttf");
        new_text.setCharacterSize(12);
        new_text.setFont(font);
        new_text.setString("Basic text");
    };

    Text(unsigned int c_size, float x, float y, string font_fileName, string text_c) {
        font.loadFromFile(font_fileName);
        new_text.setCharacterSize(c_size);
        new_text.setFont(font);
        new_text.setString(text_c);
        new_text.setOrigin(x, y);

    }

    void setText(sf::Text &text, float x, float y) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }
};

#endif //MINESWEEP_TEXT_H
