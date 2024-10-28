// this file use to handle all request from peer and tracker
#include "INFO.h"

// send data of sender
void sendData(SOCKET *sendedSocket, char* path, int pieceSize, int pieceOffset)
{
    char buffer[1024] = {0};
    ifstream readfile(path, ostream::binary);
    readfile.seekg(pieceOffset * pieceSize, std::ios::beg);
    readfile.read(buffer, pieceSize);
    buffer[pieceSize] = '\0';

    int bytesSent = send(*sendedSocket, buffer, pieceSize, 0);

    //TODO

    while (bytesSent <= 0)
    {
        printf("Send error");
        bytesSent = send(*sendedSocket, buffer, pieceSize, 0);
    }
    printf("Sent %i bytes from chunk %i \nbuffer : %s\n", bytesSent, pieceOffset, buffer);
}
// used in thread to listen all requests
void listenRequest()
{
    while (true)
    {
        SOCKET clientSocket;
        int addrLen = sizeof(listenSock.addr);
        clientSocket = accept(listenSock.sock, (struct sockaddr *)&listenSock.addr, &addrLen);

        if (clientSocket == INVALID_SOCKET)
        {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
            continue;
        }
        char buffer[1024] = {0};
        int bytesRead = recv(clientSocket, buffer, 1024, 0);

        // check request with first 10 b    its
        char requestID[11];
        strncpy(requestID, buffer, 10);
        requestID[10] = '\0';
        // receive request send file
        if (strcmp(requestID, "1111111111") == 0)
        {
            printf("Thread of server receive %i bytes\n%s\n", bytesRead, buffer);
            char* fileName  = buffer;
            int offset = 0;
            //sendData(&clientSocket, fileName, 4, 0);
            sendFileNthread(clientSocket, fileName, offset, 939171);
        }
    }
}

void sendFileNthread(SOCKET clientSocket, const char *filePath, int offset, long required) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return;
    }

    file.seekg(offset*required, std::ios::beg);
    char buffer[512000];
    while ((file.read(buffer, 512000) || file.gcount() > 0) && required > 0) {
        std::streamsize bytesRead = file.gcount();
        int totalBytesSent = 0;

        while (totalBytesSent < bytesRead) {
            int bytesSent = send(clientSocket, buffer + totalBytesSent, bytesRead - totalBytesSent, 0);
            if (bytesSent == SOCKET_ERROR) {
                std::cerr << "Error sending file chunk." << std::endl;
                return;
            }
            totalBytesSent += bytesSent;
        }
        std::cout << "Sent " << totalBytesSent << " bytes." << std::endl;
        required -= totalBytesSent;
    }
    file.close();
}