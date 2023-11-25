#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Tile.h"
#include "Sprite.h"
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_WINDOW_H
#define MINESWEEP_WINDOW_H
using namespace std;

class Board {
private:
    map<int, Tile> tiles;
    int num_mines;
public:
    Board() {}

    void drawBoard(sf::RenderWindow& gameWindow, int num_rows, int num_cols) {
        int pos = 0;
        for(int i = 0; i < (num_rows * num_cols); i++) {
            Tile new_tile;
            if(i != 0) {
                pos = pos + 21;
                new_tile.state.setPosition(pos, 0);
            }
            new_tile.state.setPosition(0, 0);
            tiles[i] = new_tile;
        }

        auto iter = tiles.begin();
    }

};



#endif //MINESWEEP_WINDOW_H
