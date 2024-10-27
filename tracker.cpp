#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <fstream>
#include <cmath>
#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024
int main(int argc, char* argv[]) {

    if (argc != 1){
    std::string op = argv[2];

    // Perform the operation based on the operator
    if (op == "-discover")
    {
        std::cout << "-discover called" << std::endl;
        //TODO: Return list of peers
    } else if (op == "-ping")
    {
        std::cout << "-ping called" << std::endl;
        //TODO: Return ping status of all peers
    } 
    }

    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket for the server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Listening on port 54000
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, SOMAXCONN);
         std::cout << "Listenning!" << std::endl;
    clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);

    std::cout << "Listenning!" << std::endl;
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to accept client connection!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received string
        std::cout << "Received: " << buffer << std::endl;
    } else if (bytesReceived == 0) {
        std::cout << "Connection closed by the client." << std::endl;
    } else {
        std::cerr << "Receive failed. Error Code: " << WSAGetLastError() << std::endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
