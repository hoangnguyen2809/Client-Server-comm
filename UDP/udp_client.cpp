#include<iostream>
#include<string>
#include<winsock2.h>
#include<chrono>

using namespace std;

const string SERVER_IP = "127.0.0.1";

int main()
{
    //Initiate Winsock
        WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Failed to initialize Winsock." << endl;
        return 1;
    }
    else
    {
        cout << "Winsock initiated." << endl;
    }

    //create Socket
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        cout << "Failed to create socket." << endl;
        return 1;
    }
    else
    {
        cout << "Client socket created" << endl;
    }

    //Connect to server address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP.c_str());

    //Send data to server
    string message = "hello";

    int bytesSent = sendto(clientSocket, message.c_str(), message.length(), 0,(struct sockaddr*)&serverAddress, sizeof(serverAddress));
    auto startTime = chrono::high_resolution_clock::now();
    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Failed to send data to server." << endl;
        return 1;
    }
    else
        cout << "Data sent successfully" << endl;

    //Receive response from server
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "Failed to receive response from server" << endl;
        return 1;
    }
    else
    {
        buffer[bytesReceived] = '\0';
        cout << "Received response from server: " << buffer << endl;
    }
    
    // Calculate RTT
    chrono::duration<double, milli> duration = endTime - startTime;
    double rtt = duration.count();
    cout << "Round Trip Time (RTT): " << rtt << " ms" << endl;

    closesocket(clientSocket);
    WSACleanup();

    system("pause");

    return 0;
}