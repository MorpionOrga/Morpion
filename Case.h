#pragma once
#include "framework.h"
#include "player.h"

class Case {
public:
    Case();
    sf::Font font;
    sf::Text letter;
    void draw(sf::RenderWindow& window, int x, int y);
    char getValue();
    void setValue(player* Player);

private:
    char value;
};
