#pragma once
#include "framework.h"
#include "grid.h"

class Message {
public:
    Message();

    void sendMove(int x, int y, int value, const std::vector<SOCKET>& sockets) ;
    void win(int value, const std::vector<SOCKET>& sockets);
    void egalite(bool value, const std::vector<SOCKET>& sockets);
    bool sendResponce;
};