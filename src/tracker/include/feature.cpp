#include "INFO.h"

// create socket, addr
sockInfo createSockAddr(char *ip, int port)
{
    SOCKET sock;
    sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP socket

    while (sock == INVALID_SOCKET)
    {
        printf("Create SOCKET error");
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP socket
    }

    addr.sin_family = AF_INET;              // IPv4
    inet_pton(AF_INET, ip, &addr.sin_addr); // IP address
    addr.sin_port = htons(port);            // Port number
    return sockInfo(sock, addr);
}
// initialize for listen
sockInfo init(char *ip, int port)
{
    sockInfo info = createSockAddr((char *)ip, port);
    if (bind(info.sock, (struct sockaddr *)&info.addr, sizeof(info.addr)) == SOCKET_ERROR)
    {
        cout << "Bind failed." << endl;
        closesocket(info.sock);
        WSACleanup();
    }
    if (listen(info.sock, 10) == SOCKET_ERROR)
    {
        cerr << "Listen failed." << endl;
        closesocket(info.sock);
        WSACleanup();
    }
    return info;
}