#include <iostream>
#include <fstream>
#include <string>
#include "Text.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>

using namespace std;
void read_cfg(int &num_rows, int &num_cols, int& num_mines);
bool WelcomeWindow(int& num_rows, int& num_cols, string& user_name, int& close_window);
void GameWindow(int num_rows, int num_cols, int& num_mines, int& game_window);



int main() {
    /* Special Variables */
    int num_rows, num_cols, num_mines;
    int close_window = 0;
    int game_window = 1;
    string user_name = "";

    /* Functions for welcome window and reading in the config */
    read_cfg(num_rows, num_cols, num_mines);
    while(WelcomeWindow(num_rows, num_cols, user_name, close_window)) {}

    /* Check if username is valid and that the game window is able to be displayed */
    if (close_window != 0) {
        return 0;
    }

    /* Run the Game Window and the actual game */
    while(game_window == 1) {
        GameWindow(num_rows, num_cols, num_mines, game_window);
    }

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
    user_nameText.new_text.setStyle(sf::Text::Bold);
    player_input.new_text.setStyle(sf::Text::Bold);
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


    /* Update Sprite textures */
    hundreths.new_sprite.setTextureRect(sf::IntRect(digit_width * hundreds_digit, 0, digit_width, 32));
    tenths.new_sprite.setTextureRect(sf::IntRect(digit_width * tens_digit, 0, digit_width, 32));
    ones.new_sprite.setTextureRect(sf::IntRect(digit_width * ones_digit, 0, digit_width, 32));
}


void GameWindow(int num_rows, int num_cols, int& num_mines, int& game_window) {
    int updated_mines = num_mines;
    Tile* tiles[num_rows][num_cols];
    sf::RenderWindow gameWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Game Window", sf::Style::Close);

    /* Texture declaration */
        /* Tile Textures */
    sf::Texture tile_hidden = TextureManager::getTexture("tile_hidden");
    sf::Texture tile_revealed = TextureManager::getTexture("tile_revealed");
    sf::Texture mine_texture = TextureManager::getTexture("mine");
    sf::Texture flag_texture = TextureManager::getTexture("flag");
        /* Other Textures */
    sf::Texture digits_texture = TextureManager::getTexture("digits");
    sf::Texture happy_face = TextureManager::getTexture("face_happy");
    sf::Texture losing_face = TextureManager::getTexture("face_lose");
    sf::Texture debug_texture = TextureManager::getTexture("debug");
    sf::Texture pause_texture = TextureManager::getTexture("pause");
    sf::Texture leader_boardTexture = TextureManager::getTexture("leaderboard");
        /* Number Textures */
    sf::Texture number_one = TextureManager::getTexture("number_1");
    sf::Texture number_two = TextureManager::getTexture("number_2");
    sf::Texture number_three = TextureManager::getTexture("number_3");
    sf::Texture number_four = TextureManager::getTexture("number_4");
    sf::Texture number_five = TextureManager::getTexture("number_5");
    sf::Texture number_six = TextureManager::getTexture("number_6");
    sf::Texture number_seven = TextureManager::getTexture("number_7");
    sf::Texture number_eight = TextureManager::getTexture("number_8");


    /* Basic Sprite Declaration */
    Sprite face_happy(happy_face, num_cols, num_rows, ((num_cols/2.0f)* 32 ) - 32, 32 * (num_rows + 0.5));
    Sprite debug(debug_texture, num_cols, num_rows, (num_cols * 32) - 304, 32 * (num_rows + 0.5));
    Sprite pause(pause_texture, num_cols, num_rows, (num_cols * 32) - 240, 32 * (num_rows + 0.5));
    Sprite leaderboard(leader_boardTexture, num_cols, num_rows, (num_cols*32) - 176, 32 * (num_rows + 0.5));


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
    Sprite hundreths(digits_texture, num_cols, num_rows, 33, (32 * (num_rows + 0.5) + 16 ));
    Sprite tenths(digits_texture, num_cols, num_rows, 54, (32 * (num_rows + 0.5) + 16 ));
    Sprite ones(digits_texture, num_cols, num_rows, 75, (32 * (num_rows + 0.5) + 16 ));

    UpdateCounterSprites(ones, tenths, hundreths, num_mines);



        /* Timer Sprites */
    Sprite timerMinutes1(digits_texture, num_cols, num_rows, (num_cols * 32) - 97, (32 * (num_rows + 0.5)) + 16);
    timerMinutes1.new_sprite.setTextureRect(counter_rect);

    Sprite timerMinutes2(digits_texture, num_cols, num_rows, (num_cols * 32) - 97 + 21, (32 * (num_rows + 0.5)) + 16);
    timerMinutes2.new_sprite.setTextureRect(counter_rect);

    Sprite timerSeconds1(digits_texture, num_cols, num_rows, (num_cols * 32) - 54, (32 * (num_rows + 0.5)) + 16);
    timerSeconds1.new_sprite.setTextureRect(counter_rect);

    Sprite timerSeconds2(digits_texture, num_cols, num_rows, (num_cols * 32) - 54 + 21, (32 * (num_rows + 0.5)) + 16);
    timerSeconds2.new_sprite.setTextureRect(counter_rect);

    /* Relative X And Y For Tiles */
        /* Map for Tiles */
    int* x = new int;
    int* y = new int;
    int* z = new int;
    *x = 0;
    *y = 0;
    *z = 0;

        /* Need to Randomize the Mines Based on The Array */
    vector<Tile*> flat_tiles;

    /* Setting Sprites and Positions */
    for(int i = 0; i < num_rows ; i++) {
        for(int j = 0; j < num_cols; j++) {
            Tile* new_tile = new Tile(tile_hidden, mine_texture, flag_texture, *z);
            new_tile->state.setPosition(*x, *y);
            new_tile->mine_sprite.setPosition(*x, *y);
            new_tile->flag_sprite.setPosition(*x, *y);
            new_tile->number_sprite.setPosition(*x, *y);

            tiles[i][j] = new_tile;
            flat_tiles.push_back(new_tile);
            *x = *x +32;
            *z = *z + 1;
        }
        *x = 0;
        *y = *y + 32;
    }

    /* Set Random Mines */
    for (int k = 0; k < num_mines; k++) {
        int random_index = rand() % flat_tiles.size();
        flat_tiles[random_index]->is_mine = true;
        flat_tiles.erase(flat_tiles.begin() + random_index);
    }

    /* Deallocation because I do not need these variables anymore */
    delete x;
    delete y;
    delete z;

    /* Need to set the adjacent tiles */
    for(int i = 0; i < num_rows ; i++) {
        for (int j = 0; j < num_cols; j++) {
            vector<Tile *> adjacent_tiles;
            if ((j == 0) && (i == 0)) {
                adjacent_tiles.push_back(tiles[i][j + 1]);
                adjacent_tiles.push_back(tiles[i + 1][j]);
                adjacent_tiles.push_back(tiles[i + 1][j + 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            } else if ((j == num_cols-1) && (i == 0)) {
                adjacent_tiles.push_back(tiles[i][j - 1]);
                adjacent_tiles.push_back(tiles[i + 1][j]);
                adjacent_tiles.push_back(tiles[i + 1][j - 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            } else if ((j == num_cols-1) && (i == num_rows-1)) {
                adjacent_tiles.push_back(tiles[i][j - 1]);
                adjacent_tiles.push_back(tiles[i - 1][j]);
                adjacent_tiles.push_back(tiles[i - 1][j - 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            } else if ((i == num_rows-1) && (j == 0)) {
                adjacent_tiles.push_back(tiles[i][j + 1]);
                adjacent_tiles.push_back(tiles[i - 1][j]);
                adjacent_tiles.push_back(tiles[i - 1][j + 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            } else if (j == 0) {
                adjacent_tiles.push_back(tiles[i][j + 1]);
                adjacent_tiles.push_back(tiles[i + 1][j]);
                adjacent_tiles.push_back(tiles[i + 1][j + 1]);
                adjacent_tiles.push_back(tiles[i - 1][j]);
                adjacent_tiles.push_back(tiles[i - 1][j + 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            } else if (i == 0) {
                adjacent_tiles.push_back(tiles[i][j + 1]);
                adjacent_tiles.push_back(tiles[i][j - 1]);
                adjacent_tiles.push_back(tiles[i + 1][j + 1]);
                adjacent_tiles.push_back(tiles[i + 1][j]);
                adjacent_tiles.push_back(tiles[i + 1][j - 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;

            } else if (j == num_cols-1) {
                adjacent_tiles.push_back(tiles[i - 1][j]);
                adjacent_tiles.push_back(tiles[i - 1][j - 1]);
                adjacent_tiles.push_back(tiles[i][j - 1]);
                adjacent_tiles.push_back(tiles[i + 1][j - 1]);
                adjacent_tiles.push_back(tiles[i + 1][j]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            } else if (i == num_rows-1) {
                adjacent_tiles.push_back(tiles[i][j + 1]);
                adjacent_tiles.push_back(tiles[i][j - 1]);
                adjacent_tiles.push_back(tiles[i - 1][j + 1]);
                adjacent_tiles.push_back(tiles[i - 1][j]);
                adjacent_tiles.push_back(tiles[i - 1][j - 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            } else {
                adjacent_tiles.push_back(tiles[i][j + 1]);
                adjacent_tiles.push_back(tiles[i][j - 1]);
                adjacent_tiles.push_back(tiles[i + 1][j + 1]);
                adjacent_tiles.push_back(tiles[i + 1][j]);
                adjacent_tiles.push_back(tiles[i + 1][j - 1]);
                adjacent_tiles.push_back(tiles[i - 1][j]);
                adjacent_tiles.push_back(tiles[i - 1][j - 1]);
                adjacent_tiles.push_back(tiles[i - 1][j + 1]);
                tiles[i][j]->adjacent_tiles = adjacent_tiles;
            }
            if(tiles[i][j]->adjacentBombs() == 1) {
                tiles[i][j]->number_sprite.setTexture(number_one);
            } else if (tiles[i][j]->adjacentBombs() == 2) {
                tiles[i][j]->number_sprite.setTexture(number_two);
            } else if (tiles[i][j]->adjacentBombs() == 3) {
                tiles[i][j]->number_sprite.setTexture(number_three);
            }  else if (tiles[i][j]->adjacentBombs() == 4) {
                tiles[i][j]->number_sprite.setTexture(number_four);
            } else if (tiles[i][j]->adjacentBombs() == 5) {
                tiles[i][j]->number_sprite.setTexture(number_five);
            } else if (tiles[i][j]->adjacentBombs() == 6) {
                tiles[i][j]->number_sprite.setTexture(number_six);
            } else if (tiles[i][j]->adjacentBombs() == 7) {
                tiles[i][j]->number_sprite.setTexture(number_seven);
            } else if (tiles[i][j]->adjacentBombs() == 8) {
                tiles[i][j]->number_sprite.setTexture(number_eight);
            } else {}
        }
    }

    while(gameWindow.isOpen()) {
        sf::Event event;
        while(gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                game_window = 0;
            }

            if(event.type == sf::Event::MouseButtonPressed) {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2i mousepos = sf::Mouse::getPosition(gameWindow);
                    if((mousepos.x <= (face_happy.new_sprite.getPosition().x + 64)) && (mousepos.x >= face_happy.new_sprite.getPosition().x)) {
                        if((mousepos.y <= (face_happy.new_sprite.getPosition().y + 64)) && (mousepos.y >= face_happy.new_sprite.getPosition().y)) {
                            /* Possibly fix this */
                            gameWindow.close();
                            GameWindow(num_rows, num_cols, num_mines, game_window);
                        }
                    }

                    if((mousepos.x <= (debug.new_sprite.getPosition().x + 64)) && (mousepos.x >= debug.new_sprite.getPosition().x)) {
                        if ((mousepos.y <= (debug.new_sprite.getPosition().y + 64)) && (mousepos.y >= debug.new_sprite.getPosition().y)) {
                            /* Implement debug button */
                        }
                    }

                            /* Variables that allow for comparing to the tile number */
                    int tile_xVal = mousepos.x / 32;
                    int tile_yVal = mousepos.y / 32;
                    int num_tiles = tile_xVal + (tile_yVal * num_cols);

                    for (int i = 0; i < num_rows; i++) {
                        for (int j = 0; j < num_cols; j++) {
                            if(num_tiles == tiles[i][j]->tile_num) {
                                if(!tiles[i][j]->is_flagged) {
                                    tiles[i][j]->updateRevealedTile(tile_revealed);
                                }
                                if(tiles[i][j]->is_mine) {
                                    for (int i = 0; i < num_rows; i++) {
                                        for (int j = 0; j < num_cols; j++) {
                                            if(tiles[i][j]->is_mine) {
                                                tiles[i][j]->updateRevealedTile(tile_revealed);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                 if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                     sf::Vector2i mousepos = sf::Mouse::getPosition(gameWindow);
                     /* Variables that allow for comparing to the tile number */
                     int tile_xVal = mousepos.x / 32;
                     int tile_yVal = mousepos.y / 32;
                     int num_tiles = tile_xVal + (tile_yVal * num_cols);

                     for (int i = 0; i < num_rows; i++) {
                         for (int j = 0; j < num_cols; j++) {
                             if(num_tiles == tiles[i][j]->tile_num) {
                                 if(!tiles[i][j]->is_revealed) {
                                     tiles[i][j]->is_flagged = true;
                                     updated_mines--;
                                     UpdateCounterSprites(ones, tenths, hundreths, updated_mines);
                                 }
                             }
                         }
                     }
                 }
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
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                if(tiles[i][j]->is_mine && tiles[i][j]->is_revealed) {
                    gameWindow.draw(tiles[i][j]->state);
                    gameWindow.draw(tiles[i][j]->mine_sprite);
                } else if (tiles[i][j]->is_flagged) {
                    gameWindow.draw(tiles[i][j]->state);
                    gameWindow.draw(tiles[i][j]->flag_sprite);
                } else if (tiles[i][j]->is_revealed && (tiles[i][j]->adjacent_tiles.size() != 0)) {
                    gameWindow.draw(tiles[i][j]->state);
                    gameWindow.draw(tiles[i][j]->number_sprite);
                } else {
                    gameWindow.draw(tiles[i][j]->state);
                }
            }
        }
        gameWindow.display();
    }

}


