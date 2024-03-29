//Empeche d'avoir certains avertissements li�s � l'utilisation de fonctions risqu�es
#define _CRT_SECURE_NO_WARNINGS

//On se lie � la biblioth�que ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#include "framework.h"
#include "player.h"
#include "grid.h"
#include "responsValue.h"
#include "Thread.h"

int xTest ;
int yTest ;
using namespace rapidjson;
std::vector<SOCKET> acceptedSockets;

CRITICAL_SECTION critical;
player Player;
Grid gridGame;
Message sendMSG;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


void handleClient(const std::string& jsonRequest , SOCKET hsocket)
{
    rapidjson::Document document;
    document.Parse(jsonRequest.c_str());

    if (!document.HasParseError() || document.IsObject()) {
        const char* messageType = document["type"].GetString();

        if (std::strcmp(messageType, "move") == 0) {
            xTest = document["x"].GetInt();
            yTest = document["y"].GetInt();
            Player.currentPlayer = document["name"].GetString();
            if (gridGame.checkWin(1))
            {
                sendMSG.win(1, acceptedSockets);
            }
            else if (gridGame.checkWin(2))
            {
                sendMSG.win(2, acceptedSockets);
            }
            else if (gridGame.isFull())
            {
                sendMSG.egalite(true , acceptedSockets);
            }
            else if (gridGame.handleEvent(&Player, xTest, yTest))
            {
                sendMSG.sendMove(xTest, yTest, gridGame.grid[xTest][yTest].getValue(), acceptedSockets);
            }
            std::cout << "Received from client: (" << xTest << ", " << yTest << ")" << std::endl;
        }
        else if (std::strcmp(messageType, "player") == 0) {
            std::string name = document["name"].GetString();
            Player.handlePlayer(name);
            std::cout << "Received from client: (" << name << ")" << std::endl;
        }
        else {
            std::cerr << "Unknown message type: " << messageType << std::endl;
        }
    }
    else {
        std::cerr << "Invalid JSON format. Expected an object." << std::endl;
    }
}


class ThreadServer : public Thread
{
public:
    ThreadServer() : Thread() {  };

    virtual bool Run() override {

        while (true) {}
        return true;
    }
    virtual bool Start() override
    {
        std::cout << "Starting..." << std::endl;
        m_threadHandle = CreateThread(nullptr, 0, Static_ThreadProc, (void*)this, 0, nullptr);
        return (m_threadHandle != nullptr);
    }

    ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = 0;
        wcex.hCursor = 0;
        wcex.hbrBackground = 0;
        wcex.lpszMenuName = 0;
        wcex.lpszClassName = L"WNDCLASS";
        wcex.hIconSm = 0;

        return RegisterClassExW(&wcex);
    }



    void InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
        hWnd = CreateWindowW(L"WNDCLASS", L"", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    }

    virtual void OnThread() override
    {
        Player.initList();

        hInstance = GetModuleHandle(0);
        MyRegisterClass(hInstance);

        InitInstance(hInstance, 0);

        // Cr�ation de la socket
        SOCKET hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (hsocket == INVALID_SOCKET) {
            printf("socket failed\n");
            WSACleanup();
        }
        else {
            std::cout << "Socket creation successful\n";
        }

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORT);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // Liaison du socket
        if (bind(hsocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            printf("Bind failed\n");
            closesocket(hsocket);
            WSACleanup();
        }
        else {
            std::cout << "Bind successful\n";
        }

        // Ecoute du socket
        if (listen(hsocket, SOMAXCONN) == SOCKET_ERROR) {
            printf("Listen failed\n");
            closesocket(hsocket);
            WSACleanup();
        }
        else {
            std::cout << "Listen successful\n";
        }

        if (WSAAsyncSelect(hsocket, hWnd, WM_ACCEPT, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR) {
            printf("WSAAsyncSelect failed\n");
            closesocket(hsocket);
            WSACleanup();

        }
        else {
            std::cout << "WSAAsyncSelect successful\n";
        }

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }

private:
    HINSTANCE hInstance;
    HWND hWnd;
};




int main() {
    int iResult;

    // initialisation de winsock
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    else {
        std::cout << "WSAStartup successful\n";
    }

    ThreadServer* threadServer = new ThreadServer();
    threadServer->Start();


    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    WSACleanup();

    DeleteCriticalSection(&critical);

    return 0;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ACCEPT:
    {
        SOCKET acceptSocket = accept(wParam, nullptr, nullptr);
        if (acceptSocket == INVALID_SOCKET) {
            std::cout << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
            WSACleanup();
            return 1;
        }
        else {
            std::cout << "Acceptation de la connexion entrante." << std::endl;
        }

        acceptedSockets.push_back(acceptSocket);  // Ajoutez le nouveau socket � la liste

        if (WSAAsyncSelect(acceptSocket, hWnd, WM_READ, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
            printf("WSAAsyncSelect failed for clientSocket\n");
            closesocket(acceptSocket);
            WSACleanup();
            return 1;
        }
        else {
            std::cout << "WSAAsyncSelect successful for clientSocket\n";
        }
        break;
    }
    case WM_READ:
    {
        // Messages des clients
        for (SOCKET acceptedSocket : acceptedSockets) {
            char buffer[4096];
            int bytesRead = recv(acceptedSocket, buffer, sizeof(buffer), 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                handleClient(buffer, acceptedSocket);
            }
            else if (bytesRead == 0 || WSAGetLastError() == WSAECONNRESET) {
                // Si le client s'est d�connect�, retirez le socket de la liste
                closesocket(acceptedSocket);
                acceptedSockets.erase(std::remove(acceptedSockets.begin(), acceptedSockets.end(), acceptedSocket), acceptedSockets.end());
            }
        }
        break;
    }
    // ... (autres cas)

    return 0;
    }
}