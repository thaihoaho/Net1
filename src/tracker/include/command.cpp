#include "INFO.h"
bool sendRequest(char *ip, int port, char *buffer, bool pri)
{
    sockInfo socket = createSockAddr(ip, port);

    if (connect(socket.sock, (sockaddr *)&socket.addr, sizeof(socket.addr)) == SOCKET_ERROR)
    {
        if (pri)
            std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    if (pri)
        printf("Pinging %s:%i with 33 bytes of data:\n", ip, port);
    char response[43] = {0};
    strcpy(response, PING_REQUEST);
    strcat(response, ping_request);

    for (int i = 0; i < 4; i++)
    {
        auto begin_time = chrono::high_resolution_clock::now();
        if (send(socket.sock, ping_request, 33, 0) == SOCKET_ERROR)
        {
            if (pri)
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            return false;
        }
        recv(socket.sock, buffer, sizeof(buffer) - 1, 0);
        auto end = std::chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> elapsed = end - begin_time;
        if (pri)
            printf("Reply from %s:%i: bytes=33 time= %.2f ms\n", ip, port, elapsed.count());
    }
    if (pri)
        printf("Ping successfully\n");
    closesocket(socket.sock);
    return true;
}