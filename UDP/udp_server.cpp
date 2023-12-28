#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    // Initiate Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Failed to initiate Winsock." << endl;
        return 1;
    } else {
        cout << "Winsock initiated." << endl;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Failed to create server socket." << endl;
        WSACleanup();
        return 1;
    } else {
        cout << "Server socket created." << endl;
    }

    // Bind IP address and port to the socket
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cout << "Failed to bind socket." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
        cout << "Bind successful." << endl;
    }

    while (true) {
        // Receive and send data
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);

        char buffer[BUFFER_SIZE];
        int bytesRead = recvfrom(serverSocket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (bytesRead == SOCKET_ERROR) {
            cout << "Failed to receive data." << endl;
            continue;
        }

        buffer[bytesRead] = '\0'; // Null-terminate the buffer

        cout << "Received data from client: " << buffer << endl;

        string response = "back at you";
        int bytesSent = sendto(serverSocket, response.c_str(), response.length(), 0, (struct sockaddr*)&clientAddress, clientAddressSize);
        if (bytesSent == SOCKET_ERROR) {
            cout << "Failed to send response." << endl;
            continue;
        }

        cout << "Response sent to client." << endl;
    }

    // Close the server socket and cleanup Winsock
    closesocket(serverSocket);
    WSACleanup();

    system("pause");
    return 0;
}
