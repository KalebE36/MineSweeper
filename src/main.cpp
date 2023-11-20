#include <iostream>
#include "text.h"
#include <SFML/Graphics.hpp>

using namespace std;

bool welcomeWindow() {
    sf::RenderWindow welcomeWindow(sf::VideoMode(1000, 800), "Minesweeper");
    Text welcome_text(48, 0, 0, "../font.ttf", "WELCOME TO MINESWEEPER!");
    welcome_text.new_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
    welcome_text.setText(welcome_text.new_text, 500, 300);

    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return false;
            }
        }

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcome_text.new_text);
        welcomeWindow.display();
    }
}

int main() {
    while(welcomeWindow()) {}
    return 0;
}