#include <iostream>
#include <fstream>
#include <string>
#include "text.h"
#include <SFML/Graphics.hpp>

using namespace std;

/* reads the data from the config file */
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



/* This function just makes sure that a usr inputs valid credentials and displays the welcome screen */
bool welcomeWindow(int& num_rows, int& num_cols, string& user_name) {
    int checknum = 0;
    int max_num = 0;
    sf::String welcome_input;
    sf::RenderWindow welcomeWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Minesweeper");

    /* Setting up Text placements and properties for the welcome screen */
    Text player_input(16, "../font.ttf", "|");
    Text user_nameText(16,  "../font.ttf", "Enter your name: ");
    Text greet_mineText(20,  "../font.ttf", "WELCOME TO MINESWEEPER!");
    greet_mineText.new_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
    user_nameText.setText(user_nameText.new_text, ((num_cols*32)/2.0f), 250);
    greet_mineText.setText(greet_mineText.new_text, ((num_cols*32)/2.0f), 200);

    /* Key events */
    while(welcomeWindow.isOpen()) {
        player_input.setText(player_input.new_text, ((num_cols*32)/2.0f), 300);
        sf::Event event;
        checknum = 0;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return false;
            }
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Enter) {
                    if(welcome_input.getSize() > 0) {
                        welcomeWindow.close();
                        user_name = welcome_input;
                        return false;
                    } else {
                        checknum = 1;
                    }
                }
            }
            if(welcome_input.getSize() > 10) {
                max_num = 1;
            }
            if (event.type == sf::Event::TextEntered) {
                if(checknum != 1 || max_num == 1) {
                    if ((event.text.unicode == '\b') && (welcome_input.getSize() > 0)) {
                        welcome_input.erase(welcome_input.getSize() -1, 1);
                        player_input.changeString(welcome_input);
                        max_num = 0;
                    } else {
                        if(max_num != 1) {
                            if (event.text.unicode >= 32 && event.text.unicode < 128) {
                                welcome_input += event.text.unicode;
                                player_input.changeString(welcome_input + "|");
                            }
                        }
                    }
                }
            }
        }

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(greet_mineText.new_text);
        welcomeWindow.draw(user_nameText.new_text);
        welcomeWindow.draw(player_input.new_text);
        welcomeWindow.display();
    }
}




int main() {
    int num_rows, num_cols, num_mines;
    string user_name = "";
    read_cfg(num_rows, num_cols, num_mines);
    while(welcomeWindow(num_rows, num_cols, user_name)) {}
    return 0;
}