#pragma once
#include "framework.h"
#include "grid.h"

class Message {
public:
    Message();

    void sendMove(int x, int y, int value, SOCKET socket) ;
    bool sendResponce;
};