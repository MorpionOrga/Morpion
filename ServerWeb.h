#pragma once

#include "framework.h"
#include "Thread.h"

#define PORT2 14889

class ServerWeb : public Thread{
public:
    ServerWeb(int port, HINSTANCE hInstance) : Thread() {};
    ~ServerWeb();

    virtual bool Run() override;
    virtual bool Start() override;
    virtual void OnThread() override;


    bool StartAsyncListening();
    void AddClientSocket(SOCKET clientSocket);
    void ResponseRequest(SOCKET clientSocket);
    void Close();

    HWND hwnd;

private:
    int port;
    SOCKET listenSocket;
    std::vector<SOCKET> clientSockets;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Fonction pour générer le contenu HTML en fonction de l'état du jeu
    std::string GenerateHtmlContent() const;
};





