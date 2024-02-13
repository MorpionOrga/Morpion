#pragma once
#include "framework.h"
#include "Case.h"
#include "player.h"

const int gridSize = 3;

class Grid {
public:

    Grid();
    bool handleEvent(player* player, int x, int y);
    void update();
    bool checkWin(char player);
    bool isFull();
    sf::Text whoPlay;
    sf::Font font;
    const char* currentPlayer;
    bool Xplay;
    Case grid[gridSize][gridSize];   
    
};
