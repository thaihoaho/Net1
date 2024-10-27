// this file use to handle all request from peer and tracker
#include "INFO.h"

// send data of sender
void sendData(SOCKET *sendedSocket, char* path, int pieceSize, int pieceOffset)
{
    char *buffer = new char[1024];
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

        // check request with first 10 bits
        char requestID[11];
        strncpy(requestID, buffer, 10);
        requestID[10] = '\0';
        // receive request send file
        if (strcmp(requestID, "1111111111") == 0)
        {
            printf("Thread of server receive %i bytes\n%s\n", bytesRead, buffer);
            sendData(&clientSocket, (char*)"files/aa.txt", 4, 0);
        }



    }
}
