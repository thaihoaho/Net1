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

long list(bool x, string filename)
{
    std::string path = "./files/";
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (fs::is_regular_file(entry.status()))
        {

            if (x == 1 && entry.path().filename().string() == filename)
            {   
                return (long)(fs::file_size(entry));
            } else
            if (x == 0)
                std::cout << "  File: " << entry.path().filename() << " - Size: "
                          << fs::file_size(entry) << " bytes" << std::endl;
        }
    }
    return 0;
}


int piecesCount(long filesize)
{
    return ceil(filesize / 512000);
}

char *generateHashinfo(string filename, long filesize, int piecesCount, int pieceSize)
{
    char *temp = new char[11]; // Allocate memory for 10 characters + null terminator
    std::strcpy(temp, "0000000000");

    return temp;
}