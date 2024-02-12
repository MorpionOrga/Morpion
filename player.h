#pragma once
#include "framework.h"

class player {
public:
    player();
    virtual ~player();
    
    void handlePlayer(std::string name);
    void addPlayer(std::string name);
    void playerLeave();
    BOOL pEmpty(std::string name);
    int getIndexFromName(std::string name);
    char getValue();

    const char* currentPlayer;
    std::string playerName;
    char value;
    SOCKET players;
    player* pList[10];
    
};

