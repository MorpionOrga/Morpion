#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include "framework.h"


class ClientHandler {
public:
    static void handleClient(const std::string& jsonRequest);
};

#endif // CLIENTHANDLER_H
