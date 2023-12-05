#include <iostream>
#include <fstream>
#include <string>
#include "Text.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Tile.h"
#include "Leaderboard.h"
#include <SFML/Graphics.hpp>

using namespace std;
void read_cfg(int &num_rows, int &num_cols, int& num_mines);
bool WelcomeWindow(int& num_rows, int& num_cols, string& user_name, int& close_window);
void revealAdjacentTiles(Tile& current_tile, sf::Texture& revealed_texture);
map<int, sf::Sprite> timerDigits(sf::Sprite digits);
void GameWindow(int& num_rows, int& num_cols, int& num_mines, int& game_window, int& game_state, string& user_name);

int main() {
    /* Special Variables */
    int num_rows, num_cols, num_mines;
    int close_window = 0;
    int game_window = 1;
    string user_name = "";
    Leaderboard leaderboardStruct(user_name);


    /* Functions for welcome window and reading in the config */
    read_cfg(num_rows, num_cols, num_mines);
    while(WelcomeWindow(num_rows, num_cols, user_name, close_window)) {}

    /* Check if username is valid and that the game window is able to be displayed */
    if (close_window != 0) {
        return 0;
    }

    /* Run the Game Window and the actual game */
    int game_state;
    while(game_window == 1) {
        GameWindow(num_rows, num_cols, num_mines, game_window, game_state, user_name);
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
    int num_flags = 0;
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

void revealAdjacentTiles(Tile& current_tile, sf::Texture& revealed_texture) {
    for(int i = 0; i < current_tile.adjacent_tiles.size(); i++) {
        if((!current_tile.adjacent_tiles.at(i)->is_flagged) && (!current_tile.adjacent_tiles.at(i)->is_mine) && (!current_tile.adjacent_tiles.at(i)->is_revealed) && (current_tile.adjacent_tiles.at(i)->adjacentBombs() == 0) && (current_tile.adjacent_mines == 0)) {
                current_tile.adjacent_tiles.at(i)->is_revealed = true;
                current_tile.adjacent_tiles.at(i)->state.setTexture(revealed_texture);
                revealAdjacentTiles(*current_tile.adjacent_tiles.at(i), revealed_texture);
        }
        if((!current_tile.adjacent_tiles.at(i)->is_flagged) && (current_tile.adjacent_tiles.at(i)->adjacentBombs() != 0) && (!current_tile.adjacent_tiles.at(i)->is_mine) && (!current_tile.adjacent_tiles.at(i)->is_revealed) && (current_tile.adjacent_mines == 0)) {
                current_tile.adjacent_tiles.at(i)->is_revealed = true;
                current_tile.adjacent_tiles.at(i)->state.setTexture(revealed_texture);
        }
    }
}

map<int, sf::Sprite> timerDigits(sf::Sprite digits){
    map<int, sf::Sprite> digitsMap;

    for(int i = 0; i < 10; i++){
        sf::IntRect rect(i*21,0,21,32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i, sprite);
    }

    return digitsMap;
}

void GameWindow(int& num_rows, int& num_cols, int& num_mines, int& game_window, int& game_state, string& user_name) {
    game_state = 1;
    int updated_mines = num_mines;
    int num_flags = 0;
    Leaderboard leaderboardStruct(user_name);
    bool game_leaderboard = false;
    bool leaderboard_check = false;
    bool pause_check = false;
    Tile* tiles[num_rows][num_cols];
    sf::RenderWindow gameWindow(sf::VideoMode((num_cols * 32), ((num_rows*32) + 100)), "Game Window", sf::Style::Close);

    /* Timer variables */
    auto start_time = chrono::high_resolution_clock::now();
    auto pause_time = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pause_time).count();
    bool paused = false;

    /* Texture declaration */
        /* Tile Textures */
    sf::Texture tile_hidden = TextureManager::getTexture("tile_hidden");
    sf::Texture tile_revealed = TextureManager::getTexture("tile_revealed");
    sf::Texture mine_texture = TextureManager::getTexture("mine");
    sf::Texture flag_texture = TextureManager::getTexture("flag");
        /* Other Textures */
    sf::Texture digits_texture = TextureManager::getTexture("digits");
    sf::Texture happy_face = TextureManager::getTexture("face_happy");
    sf::Texture win_face = TextureManager::getTexture("face_win");
    sf::Texture losing_face = TextureManager::getTexture("face_lose");
    sf::Texture debug_texture = TextureManager::getTexture("debug");
    sf::Texture pause_texture = TextureManager::getTexture("pause");
    sf::Texture play_texture = TextureManager::getTexture("play");
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
    Sprite face_lose(losing_face, num_cols, num_rows, ((num_cols/2.0f)* 32 ) - 32, 32 * (num_rows + 0.5));
    Sprite face_win(win_face, num_cols, num_rows, ((num_cols/2.0f)* 32 ) - 32, 32 * (num_rows + 0.5));
    Sprite debug(debug_texture, num_cols, num_rows, (num_cols * 32) - 304, 32 * (num_rows + 0.5));
    Sprite pause(pause_texture, num_cols, num_rows, (num_cols * 32) - 240, 32 * (num_rows + 0.5));
    Sprite play (play_texture, num_cols, num_rows, (num_cols * 32) - 240, 32 * (num_rows + 0.5));
    Sprite leaderboard(leader_boardTexture, num_cols, num_rows, (num_cols*32) - 176, 32 * (num_rows + 0.5));

    /* For the Timer, Bad code */
    sf::Sprite digit_sprite;
    digit_sprite.setTexture(digits_texture);
    map<int, sf::Sprite> digitsMap = timerDigits(digit_sprite);


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
                tiles[i][j]->adjacent_mines = 1;
            } else if (tiles[i][j]->adjacentBombs() == 2) {
                tiles[i][j]->number_sprite.setTexture(number_two);
                tiles[i][j]->adjacent_mines = 2;
            } else if (tiles[i][j]->adjacentBombs() == 3) {
                tiles[i][j]->number_sprite.setTexture(number_three);
                tiles[i][j]->adjacent_mines = 3;
            }  else if (tiles[i][j]->adjacentBombs() == 4) {
                tiles[i][j]->number_sprite.setTexture(number_four);
                tiles[i][j]->adjacent_mines = 4;
            } else if (tiles[i][j]->adjacentBombs() == 5) {
                tiles[i][j]->number_sprite.setTexture(number_five);
                tiles[i][j]->adjacent_mines = 5;
            } else if (tiles[i][j]->adjacentBombs() == 6) {
                tiles[i][j]->number_sprite.setTexture(number_six);
                tiles[i][j]->adjacent_mines = 6;
            } else if (tiles[i][j]->adjacentBombs() == 7) {
                tiles[i][j]->number_sprite.setTexture(number_seven);
                tiles[i][j]->adjacent_mines = 7;
            } else if (tiles[i][j]->adjacentBombs() == 8) {
                tiles[i][j]->number_sprite.setTexture(number_eight);
                tiles[i][j]->adjacent_mines = 8;
            } else {}
        }
    }

    while(gameWindow.isOpen()) {
        sf::Event event;

        if(game_state != 1) {
            paused = true;
        }


        while(gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                game_window = 0;
            }

            if(event.type == sf::Event::MouseButtonPressed) {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2i mousepos = sf::Mouse::getPosition(gameWindow);
                    if((mousepos.x <= (pause.new_sprite.getPosition().x + 64)) && (mousepos.x >= pause.new_sprite.getPosition().x)) {
                        if((mousepos.y <= (pause.new_sprite.getPosition().y + 64)) && (mousepos.y >= pause.new_sprite.getPosition().y)) {
                            if (game_state == 1) {
                                paused = !paused;
                                if(paused) {
                                    cout << "paused" << endl;
                                    pause_time = chrono::high_resolution_clock::now();
                                    for (int i = 0; i < num_rows; i++) {
                                        for (int j = 0; j < num_cols; j++) {
                                            tiles[i][j]->fakeRevealedTile(tile_revealed);
                                        }
                                    }
                                } else{
                                    for (int i = 0; i < num_rows; i++) {
                                        for (int j = 0; j < num_cols; j++) {
                                            if(!tiles[i][j]->is_revealed) {
                                                tiles[i][j]->restoreTile(tile_hidden);
                                            }
                                        }
                                    }
                                    auto unPausedTime = chrono::steady_clock::now();
                                    elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pause_time).count(); //Addition is necessary for when hitting the pause button more than once
                                    //cout << elapsed_paused_time % 60 << " " << endl;
                                    pause_check = false;
                                }
                            }
                        }
                    }

                    if((mousepos.x <= (face_happy.new_sprite.getPosition().x + 64)) && (mousepos.x >= face_happy.new_sprite.getPosition().x)) {
                        if((mousepos.y <= (face_happy.new_sprite.getPosition().y + 64)) && (mousepos.y >= face_happy.new_sprite.getPosition().y)) {
                            /* Possibly fix this */
                            gameWindow.close();
                            GameWindow(num_rows, num_cols, num_mines, game_window, game_state, user_name);
                        }
                    }

                    /* Implement debug button */
                    if((mousepos.x <= (debug.new_sprite.getPosition().x + 64)) && (mousepos.x >= debug.new_sprite.getPosition().x)) {
                        if ((mousepos.y <= (debug.new_sprite.getPosition().y + 64)) && (mousepos.y >= debug.new_sprite.getPosition().y)) {
                            if(!paused) {
                                for (int i = 0; i < num_rows; i++) {
                                    for (int j = 0; j < num_cols; j++) {
                                        if((tiles[i][j]->is_mine) && (!tiles[i][j]->is_debug)) {
                                            tiles[i][j]->is_debug = true;
                                            tiles[i][j]->is_revealed = true;
                                        } else if((tiles[i][j]->is_mine) && (tiles[i][j]->is_debug)) {
                                            tiles[i][j]->is_debug = false;
                                            tiles[i][j]->is_revealed = false;
                                        } else {}
                                    }
                                }
                            }
                        }
                    }

                    /* Leaderboard button */
                    if((mousepos.x <= (leaderboard.new_sprite.getPosition().x + 64)) && (mousepos.x >= leaderboard.new_sprite.getPosition().x)) {
                        if ((mousepos.y <= (leaderboard.new_sprite.getPosition().y + 64)) && (mousepos.y >= leaderboard.new_sprite.getPosition().y)) {
                            if(paused) {
                                if(game_state == 1 ) {
                                    pause_check = true;
                                }
                                if(game_state != 2) {
                                    for (int i = 0; i < num_rows; i++) {
                                        for (int j = 0; j < num_cols; j++) {
                                            tiles[i][j]->fakeRevealedTile(tile_revealed);
                                        }
                                    }
                                }
                                leaderboard_check = true;
                            } else {
                                paused = true;
                                pause_time = chrono::high_resolution_clock::now();
                                if(game_state == 1) {
                                    for (int i = 0; i < num_rows; i++) {
                                        for (int j = 0; j < num_cols; j++) {
                                            tiles[i][j]->fakeRevealedTile(tile_revealed);
                                        }
                                    }
                                }
                                leaderboard_check = true;
                            }
                        }
                    }


                            /* Variables that allow for comparing to the tile number */
                    int tile_xVal = mousepos.x / 32;
                    int tile_yVal = mousepos.y / 32;
                    int num_tiles = tile_xVal + (tile_yVal * num_cols);

                    /* Revealing adjacent tiles and mines if clicked */
                    for (int i = 0; i < num_rows; i++) {
                        for (int j = 0; j < num_cols; j++) {
                            if(num_tiles == tiles[i][j]->tile_num) {
                                if(!tiles[i][j]->is_flagged && !tiles[i][j]->is_revealed && !tiles[i][j]->is_mine && (game_state == 1)) {
                                    tiles[i][j]->updateRevealedTile(tile_revealed);
                                    revealAdjacentTiles(*tiles[i][j], tile_revealed);
                                }
                                if(tiles[i][j]->is_mine) {
                                    for (int i = 0; i < num_rows; i++) {
                                        for (int j = 0; j < num_cols; j++) {
                                            if(tiles[i][j]->is_mine) {
                                                game_state = 0;
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
                                 if(!tiles[i][j]->is_revealed && (game_state == 1)) {
                                     if(tiles[i][j]->is_flagged) {
                                         tiles[i][j]->is_flagged = false;
                                         updated_mines++;
                                         num_flags--;
                                         UpdateCounterSprites(ones, tenths, hundreths, updated_mines);
                                     } else if(num_flags != num_mines) {
                                         tiles[i][j]->is_flagged = true;
                                         updated_mines--;
                                         num_flags++;
                                         UpdateCounterSprites(ones, tenths, hundreths, updated_mines);
                                     }
                                 }
                             }
                         }
                     }
                 }
            }

        }
        int revealed_tile = 0;

        if(game_state == 1) {
            for (int i = 0; i < num_rows; i++) {
                for (int j = 0; j < num_cols; j++) {
                    if(tiles[i][j]->is_revealed && !tiles[i][j]->is_mine) {
                        revealed_tile++;
                    }
                }
            }
        }



        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count();
        int minutes;
        int seconds;

        if(!paused) {
            //enters if the game is NOT paused. This is the condition that keeps the timer from incrementing when paused.
            total_time =  total_time - elapsed_paused_time; //
            minutes = total_time / 60;
            seconds = total_time % 60;
        }
        int minutes0 = minutes / 10 % 10; //minutes index 0
        int minutes1 = minutes % 10; // minutes index 1
        int seconds0 = seconds / 10 % 10; // seconds index 0
        int seconds1 = seconds % 10; // seconds index 1

        if(((num_rows * num_cols) - num_mines) == revealed_tile) {
            float user_time = 0;
            updated_mines = 0;
            UpdateCounterSprites(ones, tenths, hundreths, updated_mines);
            game_state = 2;
            game_leaderboard = true;
            string time = to_string(minutes0) + to_string(minutes1) + ":" + to_string(seconds0) + to_string(seconds1);
            string info = time + "," + user_name;
            string temp_time = time;
            replace(temp_time.begin(), temp_time.end(), ':', '.');
            user_time = stof(temp_time);
            leaderboardStruct.writeTextFile(info, user_time);
        }

        /* Draw everything to the window */

        gameWindow.clear(sf::Color::White);

        digitsMap[minutes0].setPosition((num_cols * 32) - 97, (32 * (num_rows + 0.5)) + 16);
        gameWindow.draw(digitsMap[minutes0]);
        digitsMap[minutes1].setPosition((num_cols * 32) - 97 + 21,  (32 * (num_rows + 0.5)) + 16);
        gameWindow.draw(digitsMap[minutes1]);
        digitsMap[seconds0].setPosition( (num_cols * 32) - 54,  (32 * (num_rows + 0.5)) + 16);
        gameWindow.draw(digitsMap[seconds0]);
        digitsMap[seconds1].setPosition((num_cols * 32) - 54 + 21, (32 * (num_rows + 0.5)) + 16);
        gameWindow.draw(digitsMap[seconds1]);
        gameWindow.draw(pause.new_sprite);
        gameWindow.draw(debug.new_sprite);
        gameWindow.draw(leaderboard.new_sprite);
        gameWindow.draw(ones.new_sprite);
        gameWindow.draw(tenths.new_sprite);
        gameWindow.draw(hundreths.new_sprite);
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                if (game_state == 2) {
                    if (!tiles[i][j]->is_flagged && !tiles[i][j]->is_revealed) {
                        gameWindow.draw(tiles[i][j]->state);
                        gameWindow.draw(tiles[i][j]->flag_sprite);
                    } else if (!tiles[i][j]->is_flagged && tiles[i][j]->is_revealed) {
                        gameWindow.draw(tiles[i][j]->state);
                        gameWindow.draw(tiles[i][j]->number_sprite);
                    } else if(tiles[i][j]->is_flagged && !tiles[i][j]->is_revealed) {
                        gameWindow.draw(tiles[i][j]->state);
                        gameWindow.draw(tiles[i][j]->flag_sprite);
                    }
                } else {
                    if (tiles[i][j]->is_mine && tiles[i][j]->is_revealed && !leaderboard_check) {
                        gameWindow.draw(tiles[i][j]->state);
                        gameWindow.draw(tiles[i][j]->mine_sprite);
                    } else if ((tiles[i][j]->is_flagged && !leaderboard_check && !paused) || (tiles[i][j]->is_flagged && !leaderboard_check && game_state == 0)) {
                    gameWindow.draw(tiles[i][j]->state);
                    gameWindow.draw(tiles[i][j]->flag_sprite);
                    } else if ((tiles[i][j]->is_flagged && tiles[i][j]->is_debug && !leaderboard_check && !paused) || (tiles[i][j]->is_flagged && tiles[i][j]->is_debug && !leaderboard_check && game_state == 0)) {
                    gameWindow.draw(tiles[i][j]->state);
                    gameWindow.draw(tiles[i][j]->flag_sprite);
                    gameWindow.draw(tiles[i][j]->mine_sprite);
                    } else if ((tiles[i][j]->is_revealed && !leaderboard_check && !paused) || (tiles[i][j]->is_revealed && !leaderboard_check && game_state == 0)) {
                    gameWindow.draw(tiles[i][j]->state);
                    gameWindow.draw(tiles[i][j]->number_sprite);
                    } else if (game_state != 2) {
                    gameWindow.draw(tiles[i][j]->state);
                    }
                }
            }
        }
        if(paused && (game_state == 1) && !leaderboard_check || pause_check){
            gameWindow.draw(play.new_sprite);
        }
        if(game_state == 1) {
            gameWindow.draw(face_happy.new_sprite);
        } else if (game_state == 0) {
            gameWindow.draw(face_lose.new_sprite);
        } else  {
            gameWindow.draw(face_win.new_sprite);
        }
        gameWindow.display();

        if(game_state == 2 && game_leaderboard) {
            leaderboardStruct.displayLeaderWindow(num_cols, num_rows, leaderboard_check, game_state);
            game_leaderboard = false;
        }


        if(leaderboard_check) {
            leaderboardStruct.displayLeaderWindow(num_cols, num_rows, leaderboard_check, game_state);
            if(!pause_check) {
                paused = false;
                auto unPausedTime = chrono::steady_clock::now();
                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pause_time).count();
                for (int i = 0; i < num_rows; i++) {
                    for (int j = 0; j < num_cols; j++) {
                        if(!tiles[i][j]->is_revealed) {
                            tiles[i][j]->restoreTile(tile_hidden);
                        }
                    }
                }
            }
        }

    }
}


