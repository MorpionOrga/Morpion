#include <SFML/Graphics.hpp>
#include "Grid.h"
#include <iostream>

void game()
{
    sf::RenderWindow window(sf::VideoMode(300, 350), "Morpion");

    Grid gameGrid;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            gameGrid.handleEvent(event);
        }

        window.clear();
        gameGrid.update();
        gameGrid.draw(window);
        window.display();

        if (gameGrid.checkWin('X')) {
            std::cout << "X win" << std::endl;
            window.close();
        }
        else if (gameGrid.checkWin('O')) {
            std::cout << "O win" << std::endl;
            window.close();
        }
        else if (gameGrid.isFull()) {
            std::cout << "Egalite" << std::endl;
            window.close();
        }
    }
}