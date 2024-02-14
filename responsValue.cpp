#include "responsValue.h"

Message::Message()
{
    sendResponce = false;
}

void Message::sendMove(int x , int y ,int value , const std::vector<SOCKET>& sockets) {
    
    rapidjson::Document message;
    message.SetObject();

    message.AddMember("type", "move", message.GetAllocator());

    rapidjson::Value X;
    X.SetInt(x);
    message.AddMember("x", X, message.GetAllocator());

    rapidjson::Value Y;
    Y.SetInt(y);
    message.AddMember("y", Y, message.GetAllocator());

    rapidjson::Value Value;
    Value.SetInt(value);
    message.AddMember("value", Value, message.GetAllocator());

    // Convertit le document JSON en chaîne de caractères
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    message.Accept(writer);

    // Parcourir tous les sockets dans le vecteur et envoyer le message à chacun
    for (SOCKET socket : sockets) {
        send(socket, buffer.GetString(), buffer.GetLength(), 0);
    }
}

void Message::win(int value , const std::vector<SOCKET>& sockets)
{
    rapidjson::Document message;
    message.SetObject();

    message.AddMember("type", "win", message.GetAllocator());

    rapidjson::Value Value;
    Value.SetInt(value);
    message.AddMember("value", Value, message.GetAllocator());

    // Convertit le document JSON en chaîne de caractères
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    message.Accept(writer);

    // Parcourir tous les sockets dans le vecteur et envoyer le message à chacun
    for (SOCKET socket : sockets) {
        send(socket, buffer.GetString(), buffer.GetLength(), 0);
    }
}

void Message::egalite(bool value, const std::vector<SOCKET>& sockets)
{
    rapidjson::Document message;
    message.SetObject();

    message.AddMember("type", "egalite", message.GetAllocator());

    rapidjson::Value Value;
    Value.SetBool(value);
    message.AddMember("value", Value, message.GetAllocator());

    // Convertit le document JSON en chaîne de caractères
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    message.Accept(writer);

    // Parcourir tous les sockets dans le vecteur et envoyer le message à chacun
    for (SOCKET socket : sockets) {
        send(socket, buffer.GetString(), buffer.GetLength(), 0);
    }
}