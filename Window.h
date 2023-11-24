#include <iostream>
#include <string>
#include <vector>
#include "Tile.h"
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_WINDOW_H
#define MINESWEEP_WINDOW_H
using namespace std;

class GameWindow {
private:
    sf::RenderWindow window;
    vector<Tile> tiles;
public:
};



#endif //MINESWEEP_WINDOW_H
