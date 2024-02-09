//Empeche d'avoir certains avertissements liés à l'utilisation de fonctions risquées
#define _CRT_SECURE_NO_WARNINGS

//On se lie à la bibliothèque ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

//Include que nous allons utiliser
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"



//Définie le port
//#define PORT 80
#define PORT 14843
#define WM_ACCEPT (WM_USER + 1) 
#define WM_READ (WM_USER + 2) 

int xTest = 100;
int yTest = 100;
using namespace rapidjson;


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

void handleClient(const std::string& jsonRequest)
{
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

int main() {
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
        std::cout << "TOTO successful\n";

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
            const char* message = "Test! Again";
            send(Accept, message, strlen(message), 0);
            std::cout << "Message envoyé au client: " << message << std::endl;
        }
        break;
    }
    case WM_READ:
    {
        // message des clients
        std::cout << "TATA successful\n";
        char buffer[4096];
        int bytesRead = recv(Accept, buffer, sizeof(buffer), 0);
        buffer[bytesRead] = '\0';
        handleClient(buffer);
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
