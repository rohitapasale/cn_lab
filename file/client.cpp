
#include <iostream>
#include <fstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main() {
    // Initialize Winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create client socket
    SOCKET sockFd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9002);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockFd, (sockaddr*)&serverAddr,
                sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Connection failed! Error: "
             << WSAGetLastError() << endl;

        closesocket(sockFd);
        WSACleanup();
        return 1;
    }

    cout << "Connected to file server!" << endl;

    // Open file to send
    ifstream inFile("send_file.txt", ios::binary);

    if (!inFile) {
        cout << "Could not open send_file.txt!" << endl;

        closesocket(sockFd);
        WSACleanup();
        return 1;
    }

    char buffer[4096];

    // Read file and send chunks
    while (inFile.read(buffer, sizeof(buffer)) ||
           inFile.gcount() > 0) {

        int totalBytes = static_cast<int>(inFile.gcount());
        int totalSent = 0;

        // Ensure all bytes in the chunk are sent
        while (totalSent < totalBytes) {
            int bytesSent = send(
                sockFd,
                buffer + totalSent,
                totalBytes - totalSent,
                0
            );

            if (bytesSent == SOCKET_ERROR || bytesSent == 0) {
                cout << "Sending failed!" << endl;
                inFile.close();
                closesocket(sockFd);
                WSACleanup();
                return 1;
            }

            totalSent += bytesSent;
        }
    }

    if (inFile.bad()) {
        cout << "Error reading input file!" << endl;
        inFile.close();
        closesocket(sockFd);
        WSACleanup();
        return 1;
    }

    // Signal that no more data will be sent
    shutdown(sockFd, SD_SEND);

    cout << "File sent successfully." << endl;

    // Close file and socket
    inFile.close();
    closesocket(sockFd);
    WSACleanup();

    return 0;
}
