#include "ServerWeb.h"
#include <chrono>  
#include <WS2tcpip.h>
#include "Grid.h"

Grid myGrid;


static ServerWeb* currentInstance = nullptr;

bool ServerWeb::Run()
{
    return 1;
}

void ServerWeb::OnThread()
{
    StartAsyncListening();
}



bool ServerWeb::Start()
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = ServerWeb::WindowProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = L"MorpionWeb";

    if (!RegisterClassEx(&wc)) {
    }

    hwnd = CreateWindowEx(0, L"MorpionWeb", L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, GetModuleHandle(0), nullptr);


    // La fenêtre a été créée avec succès
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ShowWindow(hwnd, SW_HIDE);

}

ServerWeb::~ServerWeb() {
    for (SOCKET clientSocket : clientSockets) {
        closesocket(clientSocket);
    }
    WSACleanup();
    currentInstance = nullptr;
}

LRESULT CALLBACK ServerWeb::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {


    ServerWeb* currentInstance = reinterpret_cast<ServerWeb*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg)
    {
    case WM_ACCEPT:
    {
        while (true) {
            SOCKET newClientSocket = accept(currentInstance->listenSocket, nullptr, nullptr);
            if (newClientSocket == INVALID_SOCKET) {
                break;
            }
            currentInstance->AddClientSocket(newClientSocket);
        }
        break;
    }
    case WM_READ:
    {
        SOCKET clientSocket = static_cast<SOCKET>(wParam);
        currentInstance->ResponseRequest(clientSocket);

        break;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


bool ServerWeb::StartAsyncListening() {

    std::cout << "StartAsyncListening";

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();

        return false;
    }


    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) != 1) {
        std::cerr << "Failed to convert IP address." << std::endl;
        Close();
        return false;
    }

    serverAddress.sin_port = htons(port);

    if (bind(listenSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket." << std::endl;
        Close();
        return false;
    }

    if (WSAAsyncSelect(listenSocket, hwnd, WM_ACCEPT, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR) {
        std::cerr << "Failed to start asynchronous listening." << std::endl;
        Close();
        return false;
    }


    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen for incoming connections." << std::endl;
        Close();
        return false;
    }



    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen for incoming connections." << std::endl;
        closesocket(listenSocket);
        return false;
    }


    return true;
}
void ServerWeb::AddClientSocket(SOCKET clientSocket) {
    clientSockets.push_back(clientSocket);

    if (WSAAsyncSelect(clientSocket, hwnd, WM_READ, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        std::cout << "Failed to start asynchronous listening for client socket. Error code: " << errorCode << std::endl;
    }
}

std::string ServerWeb::GenerateHtmlContent() const {



    std::string htmlContent = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Morpion</title>
    <style>
        .cross {
            color: red;
        }

        .circle {
            color: blue;
        }
    </style>
</head>
<body>

    <h1>Morpion</h1>

    <!-- Tableau pour afficher le morpion -->
    <table border='1' cellpadding='10'>
    <tr>
)";

    for (int i = 0; i < gridSize; ++i) {
        // Ajouter une nouvelle ligne à chaque troisième élément
        if (i != 0) {
            htmlContent += "</tr><tr>";
        }

        for (int j = 0; j < gridSize; ++j) {
            htmlContent += "<td>";

            // Accéder à la valeur de la case dans la grille
            char value = myGrid.grid[i][j].getValue();

            // Ajouter la classe CSS en fonction du symbole
            if (value == 'X') {
                htmlContent += "<div class='cross'>X</div>";
            }
            else if (value == 'O') {
                htmlContent += "<div class='circle'>O</div>";
            }
            else {
                htmlContent += "<div>Empty</div>";
            }

            htmlContent += "</td>";
        }
    }

    // Fermer la dernière ligne
    htmlContent += "</tr>";

    // Fin du contenu HTML
    htmlContent += R"(
    </tr>
    </table>
</body>
</html>
)";

    return htmlContent;
}

void ServerWeb::ResponseRequest(SOCKET clientSocket) {



    std::string htmlContent = GenerateHtmlContent();


    std::string httpResponse = "HTTP/1.1 200 OK\r\n";
    httpResponse += "Content-Type: text/html\r\n";
    httpResponse += "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n";
    httpResponse += "\r\n";


    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);


    send(clientSocket, htmlContent.c_str(), htmlContent.size(), 0);

    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timeBuffer[26];
    ctime_s(timeBuffer, sizeof(timeBuffer), &currentTime);

}


void ServerWeb::Close() {
    closesocket(listenSocket);
}