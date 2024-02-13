//Empeche d'avoir certains avertissements liés à l'utilisation de fonctions risquées
#define _CRT_SECURE_NO_WARNINGS

//On se lie à la bibliothèque ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#include "framework.h"
#include "player.h"
#include "grid.h"
#include "responsValue.h"

int xTest ;
int yTest ;
using namespace rapidjson;

player Player;
Grid gridGame;
Message sendMSG;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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


HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"WNDCLASS", L"", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    return hWnd;
}


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
            std::cout << Player.currentPlayer << std::endl;
            if (gridGame.handleEvent(&Player, xTest, yTest))
            {
                int value = 0;
                if (gridGame.grid[xTest][yTest].getValue() == 'X')
                {
                    value = 1;
                }
                else
                {
                    value = 2;
                }
                sendMSG.sendMove(xTest, yTest, value, hsocket);
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


int main() {
    Player.initList();
    int iResult;

    HINSTANCE hInstance = GetModuleHandle(0);
    MyRegisterClass(hInstance);
    HWND hWnd = InitInstance(hInstance, 0);

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

    // Création de la socket
    SOCKET hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hsocket == INVALID_SOCKET) {
        printf("socket failed\n");
        WSACleanup();
        return 1;
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
        return 1;
    }
    else {
        std::cout << "Bind successful\n";
    }

    // Ecoute du socket
    if (listen(hsocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(hsocket);
        WSACleanup();
        return 1;
    }
    else {
        std::cout << "Listen successful\n";
    }

    if (WSAAsyncSelect(hsocket, hWnd, WM_ACCEPT, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR) {
        printf("WSAAsyncSelect failed\n");
        closesocket(hsocket);
        WSACleanup();
        return 1;
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
    
    
    
    WSACleanup();

    return 0;
}

SOCKET Accept;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_ACCEPT:
    {
        // connexion des cleitn 
        Accept = accept(wParam, nullptr, nullptr);
        if (Accept == INVALID_SOCKET) {
            std::cout << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
            closesocket(wParam);
            WSACleanup();
            return 1;
        }
        else {
            std::cout << "Acceptation de la connexion entrante." << std::endl;
        }

        if (WSAAsyncSelect(Accept, hWnd, WM_READ, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
            printf("WSAAsyncSelect failed for clientSocket\n");
            closesocket(Accept);
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
        // message des clients
        char buffer[4096];
        int bytesRead = recv(Accept, buffer, sizeof(buffer), 0);
        buffer[bytesRead] = '\0';
        handleClient(buffer , Accept);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
