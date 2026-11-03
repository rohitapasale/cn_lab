
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main() {
    // Step 1: Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "Winsock initialization failed!" << endl;
        return 1;
    }

    // Step 2: Create server socket
    SOCKET serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd == INVALID_SOCKET) {
        cout << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    // Step 3: Configure server address
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9001);

    // Step 4: Bind socket to port 9001
    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Bind failed!" << endl;
        closesocket(serverFd);
        WSACleanup();
        return 1;
    }

    // Step 5: Start listening
    if (listen(serverFd, 5) == SOCKET_ERROR) {
        cout << "Listen failed!" << endl;
        closesocket(serverFd);
        WSACleanup();
        return 1;
    }

    cout << "Hello-server listening on port 9001..." << endl;

    // Step 6: Accept client connection
    SOCKET clientFd = accept(serverFd, nullptr, nullptr);

    if (clientFd == INVALID_SOCKET) {
        cout << "Accept failed!" << endl;
        closesocket(serverFd);
        WSACleanup();
        return 1;
    }

    // Step 7: Receive message from client
    char buffer[1024] = {0};

    int bytesReceived = recv(
        clientFd, buffer, sizeof(buffer) - 1, 0
    );

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Client says: " << buffer << endl;
    } else {
        cout << "Failed to receive message!" << endl;
    }

    // Step 8: Send reply
    string reply = "Hello from server!";

    int bytesSent = send(
        clientFd,
        reply.c_str(),
        static_cast<int>(reply.size()),
        0
    );

    if (bytesSent == SOCKET_ERROR) {
        cout << "Send failed!" << endl;
    }

    // Step 9: Close sockets and clean up
    closesocket(clientFd);
    closesocket(serverFd);
    WSACleanup();

    return 0;
}
