
#include <iostream>
#include <string>
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

    // Step 2: Create client socket
    SOCKET clientFd = socket(AF_INET, SOCK_STREAM, 0);

    if (clientFd == INVALID_SOCKET) {
        cout << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    // Step 3: Configure server address
    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9001);

    // Use inet_addr instead of inet_pton
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Step 4: Connect to server
    if (connect(
            clientFd,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)
        ) == SOCKET_ERROR) {
        cout << "Connection failed! Error: "
             << WSAGetLastError() << endl;

        closesocket(clientFd);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server!" << endl;

    // Step 5: Send message to server
    string message = "Hello from client!";

    int bytesSent = send(
        clientFd,
        message.c_str(),
        static_cast<int>(message.size()),
        0
    );

    if (bytesSent == SOCKET_ERROR) {
        cout << "Sending message failed!" << endl;
        closesocket(clientFd);
        WSACleanup();
        return 1;
    }

    cout << "Message sent to server." << endl;

    // Step 6: Receive reply from server
    char buffer[1024] = {0};

    int bytesReceived = recv(
        clientFd,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Server says: " << buffer << endl;
    } else if (bytesReceived == 0) {
        cout << "Server closed the connection." << endl;
    } else {
        cout << "Receiving message failed!" << endl;
    }

    // Step 7: Close socket and clean up
    closesocket(clientFd);
    WSACleanup();

    return 0;
}
