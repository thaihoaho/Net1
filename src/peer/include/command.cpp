// this file use to handle all command that user uses
#include "INFO.h"

// receive data of reveiver
void waitData(SOCKET *socket)
{
    lock_guard<mutex> lock(mtx);
    char buffer[1024] = {0};
    int bytesRead = recv(*socket, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0)
    {
        cerr << "Receive failed: " << WSAGetLastError() << endl;
    }
    else
    {
        buffer[bytesRead] = '\0'; // Null-terminate the bufferr
        printf("Received %i bytes:\n%s\n", bytesRead, buffer);
    }
    closesocket(*socket);
}
void sendRequest(char *ip, int port, char *buffer)
{
    sockInfo socket = createSockAddr(ip, port);

    if (connect(socket.sock, (sockaddr *)&socket.addr, sizeof(socket.addr)) == SOCKET_ERROR)
    { // connect(sock2, (sockaddr*)&serverAddress2, sizeof(serverAddress2)) == SOCKET_ERROR
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        return;
    }

    if (send(socket.sock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
    {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        return;
    }
    printf("Sended request: %s\n", buffer);
    waitData(&socket.sock);
}