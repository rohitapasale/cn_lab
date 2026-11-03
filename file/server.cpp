
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "Winsock initialization failed!" << endl;
        return 1;
    }

    SOCKET serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd == INVALID_SOCKET) {
        cout << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9002);

    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(serverFd);
        WSACleanup();
        return 1;
    }

    if (listen(serverFd, 5) == SOCKET_ERROR) {
        cout << "Listen failed!" << endl;
        closesocket(serverFd);
        WSACleanup();
        return 1;
    }

    cout << "File-server listening on port 9002..." << endl;

    SOCKET clientFd = accept(serverFd, nullptr, nullptr);

    if (clientFd == INVALID_SOCKET) {
        cout << "Accept failed!" << endl;
        closesocket(serverFd);
        WSACleanup();
        return 1;
    }

    ofstream outFile("received_file.dat", ios::binary);

    if (!outFile) {
        cout << "Could not create output file!" << endl;
        closesocket(clientFd);
        closesocket(serverFd);
        WSACleanup();
        return 1;
    }

    char buffer[4096];
    int bytesRead;

    while ((bytesRead = recv(clientFd, buffer, sizeof(buffer), 0)) > 0) {
        outFile.write(buffer, bytesRead);
    }

    outFile.close();

    if (bytesRead == SOCKET_ERROR) {
        cout << "Receive failed!" << endl;
    } else {
        cout << "File received and saved as received_file.dat" << endl;
    }

    closesocket(clientFd);
    closesocket(serverFd);
    WSACleanup();

    return 0;
}
