#pragma once
#include "framework.h"

class player {
public:
    player();
    /*virtual ~player();*/

    void handlePlayer(std::string name);
    void addPlayer(std::string name);
    void playerLeave();
    BOOL pEmpty(std::string name);
    int getIndexFromName(std::string name);
    void initList();
    char getValue();

    std::string currentPlayer;
    std::string playerName;
    char value;

    player* pList[10];

};

