#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Text.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Board.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>

using namespace std;
void read_cfg(int &num_rows, int &num_cols, int& num_mines);
bool WelcomeWindow(int& num_rows, int& num_cols, string& user_name, int& close_window);
bool GameWindow(int num_rows, int num_cols, int num_mines);



int main() {
    /* Special Variables */
    int num_rows, num_cols, num_mines;
    int close_window = 0;
    string user_name = "";

    /* Functions for welcome window and reading in the config */
    read_cfg(num_rows, num_cols, num_mines);
    while(WelcomeWindow(num_rows, num_cols, user_name, close_window)) {}

    /* Check if username is valid and that the game window is able to be displayed */
    if (close_window != 0) {
        return 0;
    }

    /* Run the Game Window and the actual game */
    while(GameWindow(num_rows, num_cols, num_mines)) {}

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
    sf::RenderWindow welcomeWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Minesweeper", sf::Style::Close);

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
void UpdateCounterSprites(Sprite& ones, Sprite& tenths, Sprite& hundreths, int num_mines) {
    /* Fixed width of 21 pixels */
    int digit_width = 21;

    /* Calculate the digits based off the number of mines */

    int hundreds_digit = num_mines / 100;
    int tens_digit = (num_mines / 10) % 10;
    int ones_digit = num_mines % 10;

    std::cout << "Hundreds: " << hundreds_digit << ", Tens: " << tens_digit << ", Ones: " << ones_digit << std::endl;

    /* Update Sprite textures */
    hundreths.new_sprite.setTextureRect(sf::IntRect(digit_width * hundreds_digit, 0, digit_width, 32));
    tenths.new_sprite.setTextureRect(sf::IntRect(digit_width * tens_digit, 0, digit_width, 32));
    ones.new_sprite.setTextureRect(sf::IntRect(digit_width * ones_digit, 0, digit_width, 32));
}


bool GameWindow(int num_rows, int num_cols, int num_mines) {
    map<int, Tile> tiles;
    sf::RenderWindow gameWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Game Window", sf::Style::Close);

    /* Basic Sprite Declaration */
    Sprite face_happy("files/images/face_happy.png", num_cols, num_rows, ((num_cols/2.0f)* 32 ) - 32, 32 * (num_rows + 0.5));
    Sprite debug("files/images/debug.png", num_cols, num_rows, (num_cols * 32) - 304, 32 * (num_rows + 0.5));
    Sprite pause("files/images/pause.png", num_cols, num_rows, (num_cols * 32) - 240, 32 * (num_rows + 0.5));
    Sprite leaderboard("files/images/leaderboard.png", num_cols, num_rows, (num_cols*32) - 176, 32 * (num_rows + 0.5));

    /* IntRect and Sprites */
        /* IntRect */
    sf::IntRect counter_rect(0, 0, 21, 32);
    sf::IntRect counter_rect1(21, 0, 21, 32);
    sf::IntRect counter_rect2(42, 0, 21, 32);
    sf::IntRect counter_rect3(63, 0, 21, 32);
    sf::IntRect counter_rect4(84, 0, 21, 32);
    sf::IntRect counter_rect5(105, 0, 21, 32);
    sf::IntRect counter_rect6(126, 0, 21, 32);
    sf::IntRect counter_rect7(147, 0, 21, 32);
    sf::IntRect counter_rect8(168, 0, 21, 32);
    sf::IntRect counter_rect9(168, 0, 21, 32);

        /* Counter Sprites */
    Sprite hundreths("files/images/digits.png", num_cols, num_rows, 33, (32 * (num_rows + 0.5) + 16 ));
    Sprite tenths("files/images/digits.png", num_cols, num_rows, 54, (32 * (num_rows + 0.5) + 16 ));
    Sprite ones("files/images/digits.png", num_cols, num_rows, 75, (32 * (num_rows + 0.5) + 16 ));

    UpdateCounterSprites(ones, tenths, hundreths, num_mines);

    ones.new_sprite.setOrigin(21, 32);
    tenths.new_sprite.setOrigin(21, 32);
    hundreths.new_sprite.setOrigin(21, 32);

        /* Timer Sprites */
    Sprite timerMinutes1("files/images/digits.png", num_cols, num_rows, (num_cols * 32) - 97, (32 * (num_rows + 0.5)) + 16);
    timerMinutes1.new_sprite.setTextureRect(counter_rect);
    timerMinutes1.new_sprite.setOrigin(21, 32);

    Sprite timerMinutes2("files/images/digits.png", num_cols, num_rows, (num_cols * 32) - 97 + 21, (32 * (num_rows + 0.5)) + 16);
    timerMinutes2.new_sprite.setTextureRect(counter_rect);
    timerMinutes2.new_sprite.setOrigin(21, 32);

    Sprite timerSeconds1("files/images/digits.png", num_cols, num_rows, (num_cols * 32) - 54, (32 * (num_rows + 0.5)) + 16);
    timerSeconds1.new_sprite.setTextureRect(counter_rect);
    timerSeconds1.new_sprite.setOrigin(21, 32);

    Sprite timerSeconds2("files/images/digits.png", num_cols, num_rows, (num_cols * 32) - 54 + 21, (32 * (num_rows + 0.5)) + 16);
    timerSeconds2.new_sprite.setTextureRect(counter_rect);
    timerSeconds2.new_sprite.setOrigin(21, 32);


    Tile new_tile;
    Tile new_tile2;
    new_tile2.state.setPosition(31, 0);





    while(gameWindow.isOpen()) {
        sf::Event event;
        while(gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                return false;
            }
        }

        /* draw everything to the window */
        gameWindow.clear(sf::Color::White);
        gameWindow.draw(face_happy.new_sprite);
        gameWindow.draw(debug.new_sprite);
        gameWindow.draw(pause.new_sprite);
        gameWindow.draw(leaderboard.new_sprite);
        gameWindow.draw(ones.new_sprite);
        gameWindow.draw(tenths.new_sprite);
        gameWindow.draw(hundreths.new_sprite);
        gameWindow.draw(timerMinutes1.new_sprite);
        gameWindow.draw(timerMinutes2.new_sprite);
        gameWindow.draw(timerSeconds1.new_sprite);
        gameWindow.draw(timerSeconds2.new_sprite);
        gameWindow.draw(new_tile.state);
        gameWindow.draw(new_tile2.state);

        gameWindow.display();
    }

}


