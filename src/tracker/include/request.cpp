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

        char client_listen_ip[22] = {0};
        int client_listen_port = 0;

        // REQUEST ID
        char requestID[REQUEST_ID_LENGTH + 1];
        strncpy(requestID, buffer, REQUEST_ID_LENGTH);
        requestID[REQUEST_ID_LENGTH] = '\0';
        // receive request send file
        if (!strcmp(requestID, PUBLISH_REQUEST))
        { // publish file
            // HASHINFO
            char hashinfo[HASHINFO_LENGTH + 1];
            // more info
            char name[100];
            int filesize, piececount, piecesize;
            if (sscanf(buffer + 11, "%21s %i %10s %99s %d %d %d", client_listen_ip, &client_listen_port, hashinfo, name, &filesize, &piececount, &piecesize) != 7)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }

            mtx.lock();

            hashinfo[HASHINFO_LENGTH] = '\0';
            printf("-------Receive PUBLISH request from %s:%i-------\n", client_listen_ip, client_listen_port);

            // check file has been exist in system
            bool check = false;
            for (const auto &item : listmap)
            {
                if (strcmp(item->hashinfo, hashinfo) == 0)
                {
                    printf("%s:%i>>File with hashinfo: %s has been existed in system\n", client_listen_ip, client_listen_port, hashinfo);
                    send(clientSocket, "file existed", 10, 0);
                    check = true;
                    break;
                }
            }
            if (check)
            {
                mtx.unlock();
                continue;
            }

#ifdef DEBUG
            printf("%s:%i>>Buffer: %s, Name: %s, Hashinfo: %s, Filesize: %d, Piececount: %d, Piecesize: %d\n", client_listen_ip, client_listen_port, buffer, name, hashinfo, filesize, piececount, piecesize);
#endif

            // add into hashtable
            hashtable[hashinfo].push_back(make_pair(client_listen_ip, client_listen_port));

            listmap.push_back(new mapinfo(hashinfo, strdup(name), filesize, piececount, piecesize));

            send(clientSocket, "OK", 2, 0);
            mtx.unlock();
        }
        else if (!strcmp(requestID, FETCH_REQUEST))
        { // get all files on system
            if (sscanf(buffer + 11, "%21s %i", client_listen_ip, &client_listen_port) != 2)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }
            mtx.lock();
            printf("-------Receive FETCH request from %s:%i-------\n", client_listen_ip, client_listen_port);

            if (listmap.empty())
            {
                send(clientSocket, "Don't exist files in system.", 29, 0);
                mtx.unlock();
                continue;
            }

            char response[1024] = {0};
            snprintf(response, 11, "%s", requestID);
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
            printf("%s:%i>>Buffer: %s\n", client_listen_ip, client_listen_port, buffer);
            printf("%s:%i>>Response: %s\n", client_listen_ip, client_listen_port, response);
#endif
            send(clientSocket, response, 1024, 0);
            mtx.unlock();
        }
        else if (!strcmp(requestID, DOWN_REQUEST))
        {
            // HASHINFO
            char hashinfo[HASHINFO_LENGTH + 1];
            if (sscanf(buffer + 10, "%10s %21s %i", hashinfo, client_listen_ip, &client_listen_port) != 3)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }

            mtx.lock();

            hashinfo[HASHINFO_LENGTH] = '\0';
            printf("-------Receive DOWNLOAD request from %s:%i with file %s-------\n", client_listen_ip, client_listen_port, hashinfo);

            char response[1024] = {0};
            strncpy(response, requestID, 10);
            mapinfo *item;
            for (mapinfo *m : listmap)
            {
                if (!strcmp(hashinfo, m->hashinfo))
                {
                    item = m;
                    strcat(response, " ");
                    strcat(response, m->hashinfo);
                    strcat(response, " ");
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

            // check file has been exist in hashtable
            auto iterator = hashtable.find(hashinfo);
            if (iterator == hashtable.end())
            {
                send(clientSocket, "File not found", 1024, 0);
                mtx.unlock();
                continue;
            }
            strcat(response, " ");
            strcat(response, to_string((*iterator).second.size()).c_str());
            bool check = true;
            for (const auto &item : (*iterator).second)
            {
                if (strcmp(item.first, client_listen_ip) == 0 && item.second == client_listen_port)
                {
                    check = false;
                }
                strcat(response, " ");
                strcat(response, item.first);
                strcat(response, " ");
                strcat(response, to_string(item.second).c_str());
            }

            if (check)
                (*iterator).second.push_back(make_pair(client_listen_ip, client_listen_port));

#ifdef DEBUG
            printf("%s:%i>>Buffer: %s\n", client_listen_ip, client_listen_port, buffer);
            printf("%s:%i>>Response: %s\n", client_listen_ip, client_listen_port, response);
#endif

            send(clientSocket, response, 1024, 0);
            mtx.unlock();
        }
    }
}
