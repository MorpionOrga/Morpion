#include "Server.h"
#include "ClientHandler.h"

SOCKET Server::Accept = INVALID_SOCKET;

Server::Server() : hWnd(nullptr), hsocket(INVALID_SOCKET) {}

Server::~Server() {
    WSACleanup();
}

void Server::start() {
    int iResult;

    HINSTANCE hInstance = GetModuleHandle(0);
    MyRegisterClass(hInstance);
    hWnd = InitInstance(hInstance, 0);

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }
    else {
        std::cout << "WSAStartup successful\n";
    }

    hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hsocket == INVALID_SOCKET) {
        printf("socket failed\n");
        WSACleanup();
        return;
    }
    else {
        std::cout << "Socket creation successful\n";
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(hsocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(hsocket);
        WSACleanup();
        return;
    }
    else {
        std::cout << "Bind successful\n";
    }

    if (listen(hsocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(hsocket);
        WSACleanup();
        return;
    }
    else {
        std::cout << "Listen successful\n";
    }

    if (WSAAsyncSelect(hsocket, hWnd, WM_ACCEPT, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR) {
        printf("WSAAsyncSelect failed\n");
        closesocket(hsocket);
        WSACleanup();
        return;
    }
    else {
        std::cout << "WSAAsyncSelect successful\n";
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

ATOM Server::MyRegisterClass(HINSTANCE hInstance) {
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

HWND Server::InitInstance(HINSTANCE hInstance, int nCmdShow) {
    return CreateWindowW(L"WNDCLASS", L"", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
}

LRESULT CALLBACK Server::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
        ClientHandler::handleClient(buffer);
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

