// this file use to handle all request from peer and tracker
#include "INFO.h"

// send data of sender
void sendData(SOCKET *sendedSocket, char *path, int pieceSize, int pieceOffset)
{
    char buffer[1024] = {0};
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
        char buffer[1024] = {0};
        int bytesRead = recv(clientSocket, buffer, 1024, 0);

        sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        getpeername(clientSocket, (struct sockaddr *)&addr, &addr_len);
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));
        int client_port = ntohs(addr.sin_port);

        // REQUEST ID
        char requestID[REQUEST_ID_LENGTH + 1];
        strncpy(requestID, buffer, REQUEST_ID_LENGTH);
        requestID[REQUEST_ID_LENGTH] = '\0';
        // receive request send file
        if (!strcmp(requestID, PUBLISH_REQUEST))
        { // publish file
            // HASHINFO
            char hashinfo[HASHINFO_LENGTH + 1];
            strncpy(hashinfo, buffer + REQUEST_ID_LENGTH, HASHINFO_LENGTH);
            hashinfo[HASHINFO_LENGTH] = '\0';
            printf("Receive PUBLISH request from %s:%i\n", client_ip, client_port);
            // more info
            char name[100];
            int filesize, piececount, piecesize;

            if (!sscanf(buffer + 20, "%99s %d %d %d", name, &filesize, &piececount, &piecesize) == 4)
            {
                printf("Failed to parse remaining information.\n");
                continue;
            }

#ifdef DEBUG
            printf("Buffer: %s\n", buffer);
            printf("Name: %s\n", name);
            printf("Hashinfo: %s\n", hashinfo);
            printf("Filesize: %d\n", filesize);
            printf("Piececount: %d\n", piececount);
            printf("Piecesize: %d\n", piecesize);
#endif

            // add into hashtable
            hashtable[hashinfo].push_back(make_pair(client_ip, client_port));

            listmap.push_back(new mapinfo(hashinfo, strdup(name), filesize, piececount, piecesize));
        }
        else if (!strcmp(requestID, FETCH_REQUEST))
        { // get all files on system
            printf("Receive FETCH request from %s:%i\n", client_ip, client_port);
            char response[1024] = {0};
            snprintf(response, 10, "%s", requestID);
            for (mapinfo *m : listmap)
            {
                strcat(response, " ");
                strcat(response, m->name);

                strcat(response, " ");
                strcat(response, m->hashinfo);

                strcat(response, " ");
                strcat(response, to_string(m->filesize).c_str());
            }
            strcat(response, "\0");
            send(clientSocket, response, 1024, 0);

#ifdef DEBUG
            printf("Buffer: %s\n", buffer);
            printf("Response: %s\n", response);
#endif
        }
        else if (!strcmp(requestID, DOWN_REQUEST))
        {
            // HASHINFO
            char hashinfo[HASHINFO_LENGTH + 1];
            strncpy(hashinfo, buffer + REQUEST_ID_LENGTH, HASHINFO_LENGTH);
            hashinfo[HASHINFO_LENGTH] = '\0';

            printf("Receive DOWNLOAD request from %s:%i with file %s\n", client_ip, client_port, hashinfo);

            char *response = new char[1014];
            strncpy(response, requestID, 10);
            for (mapinfo *m : listmap)
            {
                if (!strcmp(hashinfo, m->hashinfo))
                {
                    strcat(response, m->hashinfo);
                    strcat(response, m->name);
                    strcat(response, " ");
                    strcat(response, to_string(m->filesize).c_str());
                    strcat(response, " ");
                    strcat(response, to_string(m->piececount).c_str());
                    strcat(response, " ");
                    strcat(response, to_string(m->piecesize).c_str());
                    break;
                }
            }

#ifdef DEBUG
            printf("Buffer: %s\n", buffer);
            printf("Response: %s\n", response);
#endif

            send(clientSocket, response, 1024, 0);
        }
    }
}
