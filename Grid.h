#include "Case.h"
#include "player.h"

const int gridSize = 3;

class Grid {
public:
    Grid();
    void handleEvent(sf::Event event);
    void draw(sf::RenderWindow& window);
    void update();
    bool checkWin(char player);
    bool isFull();
    sf::Text whoPlay;
    sf::Font font;
    const char* currentPlayer;

private:
    Case grid[gridSize][gridSize];
    bool Xplay;
    player Player;
};
