#ifndef SERVER_H
#define SERVER_H

#include "framework.h"

class Server {
public:
    Server();
    ~Server();
    void start();

private:
    WNDCLASSEXW wcex;
    HWND hWnd;
    WSADATA wsaData;
    SOCKET hsocket;
    static SOCKET Accept;

    ATOM MyRegisterClass(HINSTANCE hInstance);
    HWND InitInstance(HINSTANCE hInstance, int nCmdShow);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // SERVER_H


