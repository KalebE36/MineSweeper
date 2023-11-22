#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_TEXT_H
#define MINESWEEP_TEXT_H
using namespace std;



struct Text {
    sf::Text new_text;
    sf::Font font;
    sf::FloatRect text_rect;

    Text() {
        font.loadFromFile("../font.ttf");
        new_text.setCharacterSize(12);
        new_text.setFont(font);
        new_text.setString("Basic text");
    };

    Text(unsigned int c_size, string font_fileName, string text_c) {
        font.loadFromFile(font_fileName);
        new_text.setCharacterSize(c_size);
        new_text.setFont(font);
        new_text.setString(text_c);

    }

    void setText(sf::Text &text, float x, float y) {
        text_rect = text.getLocalBounds();
        text.setOrigin(text_rect.left + text_rect.width / 2.0f, text_rect.top + text_rect.height / 2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

    void changeString(string text_c) {
        new_text.setString(text_c);
    }

};

#endif //MINESWEEP_TEXT_H
