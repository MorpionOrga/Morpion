#include "Grid.h"

Grid::Grid() : Xplay(true)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            grid[i][j] = Case();
        }
    }
}

bool Grid::handleEvent(player* player, int x, int y)
{
    if (grid[x][y].getValue() == 0 && !checkWin('X') && !checkWin('O'))
    {
        if (player->currentPlayer == player->pList[0]->playerName && Xplay) {
            grid[x][y].setValue(player);
            update();
            return true;
        }
        else if (player->currentPlayer == player->pList[1]->playerName && !Xplay) {
            grid[x][y].setValue(player);
            update();
            return true;
        }
        return false;
    }
    return false;
}

void Grid::update()
{
    Xplay = !Xplay;
}

bool Grid::checkWin(int value)
{
    // Vérification des lignes
    for (int i = 0; i < gridSize; ++i)
    {
        if (grid[i][0].getValue() == value && grid[i][1].getValue() == value && grid[i][2].getValue() == value)
        {
            return true;
        }
    }

    // Vérification des colonnes
    for (int j = 0; j < gridSize; ++j)
    {
        if (grid[0][j].getValue() == value && grid[1][j].getValue() == value && grid[2][j].getValue() == value)
        {
            return true;
        }
    }

    // Vérification des diagonales
    if (grid[0][0].getValue() == value && grid[1][1].getValue() == value && grid[2][2].getValue() == value)
    {
        return true;
    }

    if (grid[0][2].getValue() == value && grid[1][1].getValue() == value && grid[2][0].getValue() == value)
    {
        return true;
    }

    return false;
}

bool Grid::isFull()
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (grid[i][j].getValue() == ' ')
            {
                return false;
            }
        }
    }
    return true;
}

