#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

using namespace std;

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
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cout << "Failed to bind socket." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
        cout << "Bind successful." << endl;
    }

    // Listen to the incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cout << "Failed to listen for client connections." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
        cout << "Start listening." << endl;
    }

    while (true) {
        // Accept client connections
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            cout << "Failed to accept client connection." << endl;
            continue;
        } else {
            cout << "Client connection accepted." << endl;
        }

        while (true) {
            // Receive the message from the client
            char buffer[1024];
            int bytesRead = recv(clientSocket, buffer, 1024, 0);
            if (bytesRead == SOCKET_ERROR) {
                cout << "Failed to receive data from client." << endl;
                closesocket(clientSocket);
                break;
            } else if (bytesRead == 0) {
                cout << "Client disconnected." << endl;
                closesocket(clientSocket);
                break;
            } else {
                cout << "Received data from the client: " << buffer << endl;

                // Reply to the message
                string receivedMessage(buffer, bytesRead);
                if (receivedMessage == "hello") {
                    string response = "back at you";
                    int bytesSent = send(clientSocket, response.c_str(), response.length(), 0);
                    if (bytesSent == SOCKET_ERROR) {
                        cout << "Failed to respond to the client." << endl;
                    } else {
                        cout << "Successful response." << endl;
                    }
                }
            }
        }
    }

    // Close the server socket and cleanup Winsock
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
