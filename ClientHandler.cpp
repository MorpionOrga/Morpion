#include "ClientHandler.h"


void ClientHandler::handleClient(const std::string& jsonRequest) {
    rapidjson::Document document;
    document.Parse(jsonRequest.c_str());

    if (document.HasParseError()) {
        std::cerr << "Error parsing JSON: " << GetParseError_En(document.GetParseError()) << std::endl;
        return;
    }

    if (document.IsObject()) {
        const char* messageType = document["type"].GetString();

        if (std::strcmp(messageType, "move") == 0) {
            int x = document["x"].GetInt();
            int y = document["y"].GetInt();

            std::cout << "Received from client: (" << x << ", " << y << ")" << std::endl;
        }
        else {
            std::cerr << "Unknown message type: " << messageType << std::endl;
        }
    }
    else {
        std::cerr << "Invalid JSON format. Expected an object." << std::endl;
    }
}
