#include <iostream>
#include <fstream>
#include <string>
#include "text.h"
#include <SFML/Graphics.hpp>

using namespace std;

void read_cfg (int &num_rows, int &num_cols, int& num_mines) {
    string temp1;
    ifstream stream("../config.cfg");
    getline(stream, temp1);
    num_cols = stoi(temp1);
    getline(stream, temp1);
    num_rows = stoi(temp1);
    getline(stream, temp1);
    num_mines = stoi(temp1);

}

bool welcomeWindow(int& num_rows, int& num_cols) {
    sf::RenderWindow welcomeWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Minesweeper");
    Text user_nameText(16, 0, 0, "../font.ttf", "Enter your name: ");
    user_nameText.setText(user_nameText.new_text, 400, 300);
    Text greet_mineText(20, 0, 0, "../font.ttf", "WELCOME TO MINESWEEPER!");
    greet_mineText.new_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
    greet_mineText.setText(greet_mineText.new_text, 400, 200);
    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return false;
            }
        }

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(greet_mineText.new_text);
        welcomeWindow.draw(user_nameText.new_text);
        welcomeWindow.display();
    }
}

int main() {
    int num_rows, num_cols, num_mines;
    read_cfg(num_rows, num_cols, num_mines);
    while(welcomeWindow(num_rows, num_cols)) {}
    return 0;
}