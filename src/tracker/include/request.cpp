// this file use to handle all request from peer and tracker
#include "INFO.h"
#include "../file.h"

// send data of sender
void sendData(SOCKET *sendedSocket, char *path, int pieceSize, int pieceOffset)
{
    char *buffer = new char[1024];
    ifstream readfile(path, ostream::binary);
    readfile.seekg(pieceOffset * pieceSize, std::ios::beg);
    readfile.read(buffer, pieceSize);
    buffer[pieceSize] = '\0';

    int bytesSent = send(*sendedSocket, buffer, pieceSize, 0);

    // TODO

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
        char *buffer = new char[1024];
        int bytesRead = recv(clientSocket, buffer, 1024, 0);

        
        // REQUEST ID
        char requestID[REQUEST_ID_LENGTH + 1];
        strncpy(requestID, buffer, REQUEST_ID_LENGTH);
        requestID[REQUEST_ID_LENGTH] = '\0';
        // receive request send file
        if (!strcmp(requestID, PUBLISH_REQUEST))
        { // publish file
            int buffersize = sizeof(buffer) / sizeof(buffer[0]);
            // HASHINFO
            char hashinfo[HASHINFO_LENGTH + 1];
            strncpy(hashinfo, buffer + REQUEST_ID_LENGTH, HASHINFO_LENGTH);
            hashinfo[HASHINFO_LENGTH] = '\0';
            printf("Receive publish request with hashinfo: %s\n", buffer);
            // more info
            // char *remaining = buffer + REQUEST_ID_LENGTH + HASHINFO_LENGTH;
            // char *name = strtok(remaining, " ");
            // char *filesize = strtok(nullptr, " ");
            // char *piececount = strtok(nullptr, " ");
            // char *piecesize = strtok(nullptr, " ");
            // char *fileinfo = strtok(nullptr, " ");

            // hashtable[hashinfo].push_back(new peerAddress(client_ip, client_port));
            // ADD INFO DATA BASE FILE WITH HASH CODE

            // DO HERE
        }
        else if (!strcmp(requestID, FETCH_REQUEST))
        { // get all files on system
          // DO HERE
            // char buffer[1024] = {0};

            // if(send(*clientSocket,buffer,1024,0) == SOCKET_ERROR)
            // {
            //     std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            //     return;
            // }

            printf("reveice :%s", buffer);
        }
        else if (!strcmp(requestID, DOWN_REQUEST))
        {
            int buffersize = sizeof(buffer) / sizeof(buffer[0]);
            // HASHINFO
            char hashinfo[HASHINFO_LENGTH + 1];
            strncpy(hashinfo, buffer + REQUEST_ID_LENGTH, HASHINFO_LENGTH);
            hashinfo[HASHINFO_LENGTH] = '\0';
        }
    }
}
