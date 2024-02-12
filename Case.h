#include <SFML/Graphics.hpp>
#include "player.h"

class Case {
public:
    Case();
    sf::Font font;
    sf::Text letter;
    void draw(sf::RenderWindow& window, int x, int y);
    char getValue();
    void setValue();

private:
    player Player;
    char value;
};
