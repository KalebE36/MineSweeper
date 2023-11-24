#include <iostream>
#include <fstream>
#include <string>
#include "Text.h"
#include "TextureManager.h"
#include "Window.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>

using namespace std;
void read_cfg(int &num_rows, int &num_cols, int& num_mines);
bool WelcomeWindow(int& num_rows, int& num_cols, string& user_name, int& close_window);
bool GameWindow(int num_rows, int num_cols);






int main() {
    /* Special Variables */
    int num_rows, num_cols, num_mines;
    int check_num = 0;
    string user_name = "";

    /* Functions for welcome window and reading in the config */
    read_cfg(num_rows, num_cols, num_mines);
    while(WelcomeWindow(num_rows, num_cols, user_name, check_num)) {}

    /* Check if username is valid and that the game window is able to be displayed */
    if (check_num != 0) {
        return 0;
    }

    /* Run the Game Window and the actual game */
    while(GameWindow(num_rows, num_cols)) {}

    return 0;
}

/* reads the data from the config file */
void read_cfg (int &num_rows, int &num_cols, int& num_mines) {
    string temp1;
    ifstream stream("files/board_config.cfg");
    getline(stream, temp1);
    num_cols = stoi(temp1);
    getline(stream, temp1);
    num_rows = stoi(temp1);
    getline(stream, temp1);
    num_mines = stoi(temp1);

}



/* This function just makes sure that a usr inputs valid credentials and displays the welcome screen */
/* Could probably use a class for the welcome screen */
bool WelcomeWindow(int& num_rows, int& num_cols, string& user_name, int& close_window) {
    int checknum = 0;
    int max_num = 0;
    sf::String welcome_input;
    sf::RenderWindow welcomeWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Minesweeper");

    /* Setting up Text placements and properties for the welcome screen */
    Text player_input(18, "files/font.ttf", "|");
    Text user_nameText(20,  "files/font.ttf", "Enter your name: ");
    Text greet_mineText(24,  "files/font.ttf", "WELCOME TO MINESWEEPER!");
    greet_mineText.new_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
    user_nameText.setText(user_nameText.new_text, ((num_cols*32)/2.0f), ((((num_rows*32) + 100))/2.0f) - 75 );
    greet_mineText.setText(greet_mineText.new_text, ((num_cols*32)/2.0f), ((((num_rows*32) + 100))/2.0f) - 150);
    player_input.new_text.setFillColor(sf::Color::Yellow);

    /* Key events */
    while(welcomeWindow.isOpen()) {
        player_input.setText(player_input.new_text, ((num_cols*32)/2.0f), ((((num_rows*32) + 100))/2.0f) - 45);
        sf::Event event;
        checknum = 0;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
                close_window = 1;
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
            if(welcome_input.getSize() > 9) {
                max_num = 1;
            }
            if (event.type == sf::Event::TextEntered) {
                if(checknum != 1 || max_num == 1) {
                    if ((event.text.unicode == '\b') && (welcome_input.getSize() > 0)) {
                        welcome_input.erase(welcome_input.getSize() - 1, 1);
                        player_input.changeString(welcome_input + "|");
                        max_num = 0;
                    } else {
                        if(max_num != 1) {
                            if(welcome_input.getSize() >= 1) {
                                if ((event.text.unicode >= 65) && (event.text.unicode < 123) ) {
                                    if ((event.text.unicode < 91) || (event.text.unicode > 96)){
                                        welcome_input += (char)tolower(event.text.unicode);
                                        player_input.changeString(welcome_input + "|");
                                    }
                                }
                            } else {
                                if ((event.text.unicode >= 65) && (event.text.unicode < 123) ) {
                                    if ((event.text.unicode < 91) || (event.text.unicode > 96)){
                                        welcome_input += (char)toupper(event.text.unicode);
                                        player_input.changeString(welcome_input + "|");
                                    }
                                }
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

bool GameWindow(int num_rows, int num_cols) {
    sf::RenderWindow gameWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Game Window");
    while(gameWindow.isOpen()) {
        sf::Event event;
        while(gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                return false;
            }
        }
    }
    gameWindow.clear();
    gameWindow.display();
}


