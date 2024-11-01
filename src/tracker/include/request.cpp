// this file use to handle all request from peer and tracker
#include "INFO.h"

void add_into_hashtable(vector<pair<char *, int>> *vect, char *ip, int port)
{
    bool exist = true;
    for (auto &iter : *vect)
    {
        if (strcmp(iter.first, ip) == 0 && port == iter.second)
        {
            exist = false;
            break;
        }
    }
    if (exist)
        vect->push_back(make_pair(ip, port));
}

// used in thread to listen all requests
void listenRequest(sockInfo *listenSock)
{
    while (running)
    {
        SOCKET clientSocket;
        int addrLen = sizeof(listenSock->addr);
        clientSocket = accept(listenSock->sock, (struct sockaddr *)&listenSock->addr, &addrLen);

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
        if (!strcmp(requestID, REGISTER_REQUEST))
        {
            char password[41] = {0};
            if (sscanf(buffer + 11, "%21[^:]:%i %40s", client_listen_ip, &client_listen_port, password) != 3)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }

            mtx.lock();
            bool registed = false;
            for (auto &iter : list_peer_info)
            {
                if (!strcmp(client_listen_ip, iter->ip) && client_listen_port == iter->port)
                {
                    registed = true;
                    break;
                }
            }
            if (registed)
            {
                send(clientSocket, "User has been registed!!", 25, 0);
                mtx.unlock();
                continue;
            }

            list_peer_info.push_back(new peerinfo(client_listen_ip, client_listen_port, password));

            // TODO: add into txt file

            send(clientSocket, "OK", 3, 0);
            mtx.unlock();
        }
        else if (!strcmp(requestID, SIGNIN_REQUEST))
        {
            char password[41] = {0};
            if (sscanf(buffer + 11, "%21[^:]:%i %40s", client_listen_ip, &client_listen_port, password) != 3)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }

            bool check = true;
            mtx.lock();
            for (auto &iter : list_peer_info)
            {
                if (!strcmp(client_listen_ip, iter->ip) && client_listen_port == iter->port && !strcmp(password, iter->password))
                {
                    check = false;
                    if (!strcmp(password, iter->password))
                    {
                        list_active_peer.push_back(make_pair(client_listen_ip, client_listen_port));
                        send(clientSocket, "OK", 3, 0);
                    }
                    else
                        send(clientSocket, "Your password incorrect!!", 26, 0);
                    break;
                }
            }
            if (check)
            {
                send(clientSocket, "Didn't file user. Make sure you have been registed!", 52, 0);
            }
            mtx.unlock();
        }
        else if (!strcmp(requestID, SIGNOUT_REQUEST))
        {
            if (sscanf(buffer + 11, "%21[^:]:%i", client_listen_ip, &client_listen_port) != 2)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }

            mtx.lock();
            // check in list_actice_peer to remove it
            bool isLogin = false;
            for (int i = 0; i < list_active_peer.size(); i++)
            {
                if (!strcmp(list_active_peer[i].first, client_listen_ip) && client_listen_port == list_active_peer[i].second)
                {
                    list_active_peer.erase(list_active_peer.begin() + i);
                    isLogin = true;
                    break;
                }
            }
            // check in hashtable to remove each file the has
            for (auto &iter : hashtable)
            {
                vector<pair<char *, int>> *vect;
                int size = vect->size();
                for (int in = 0; in < size; in++)
                {
                    if (!strcmp((*vect)[in].first, client_listen_ip) && client_listen_port == (*vect)[in].second)
                    {
                        if (size == 1)
                            vect->clear();
                        else
                            vect->erase(vect->begin() + in);
                        break;
                    }
                }
            }

            if (!isLogin)
            {
                send(clientSocket, "Something wrong!! Make sure you have been login!!", 50, 0);
            }
            else
            {
                send(clientSocket, "OK", 3, 0);
            }
            mtx.unlock();
        }
        else if (!strcmp(requestID, PUBLISH_REQUEST))
        { // publish file
            // HASHINFO
            char hashinfo[HASHINFO_LENGTH + 1];
            // more info
            char name[100];
            int filesize, piececount, piecesize;
            if (sscanf(buffer + 11, "%21s %i %11s %99s %d %d %d", client_listen_ip, &client_listen_port, hashinfo, name, &filesize, &piececount, &piecesize) != 7)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }

            mtx.lock();

            hashinfo[HASHINFO_LENGTH] = '\0';
            printf("-------Receive PUBLISH request from %s:%i-------\n", client_listen_ip, client_listen_port);

            // add into hashtable

            // check file has been exist in system
            if (!hashtable[hashinfo].empty())
            {
                printf("%s:%i>>File with hashinfo: %s has been existed in system\n", client_listen_ip, client_listen_port, hashinfo);
                send(clientSocket, "File existed", 10, 0);

                add_into_hashtable(&hashtable[hashinfo], client_listen_ip, client_listen_port);

                mtx.unlock();
                continue;
            }

            add_into_hashtable(&hashtable[hashinfo], client_listen_ip, client_listen_port);
#ifdef DEBUG
            printf("%s:%i>>Buffer: %s\nName: %s, Hashinfo: %s, Filesize: %d, Piececount: %d, Piecesize: %d\n", client_listen_ip, client_listen_port, buffer, name, hashinfo, filesize, piececount, piecesize);
#endif

            listmap.push_back(new mapinfo(strdup(hashinfo), strdup(name), filesize, piececount, piecesize));

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
            if (sscanf(buffer + 10, "%11s %21s %i", hashinfo, client_listen_ip, &client_listen_port) != 3)
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
                send(clientSocket, "File not found", 15, 0);
                mtx.unlock();
                continue;
            }
            bool check = true;
            strcat(response, " ");
            strcat(response, to_string(hashtable[hashinfo].size()).c_str());
            // list peer has hashinfo
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
            {
                (*iterator).second.push_back(make_pair(client_listen_ip, client_listen_port));
            }
            // check active peer
            bool inlist = true;
            for (auto &iter : list_active_peer)
            {
                if (!strcmp(iter.first, client_listen_ip) && iter.second == client_listen_port)
                {
                    inlist = false;
                    break;
                }
            }
            if (inlist)
                list_active_peer.push_back(make_pair(client_listen_ip, client_listen_port));

#ifdef DEBUG
            printf("%s:%i>>Buffer: %s\n", client_listen_ip, client_listen_port, buffer);
            printf("%s:%i>>Response: %s\n", client_listen_ip, client_listen_port, response);
#endif

            send(clientSocket, response, 1024, 0);
            mtx.unlock();
        }
        else if (!strcmp(requestID, DELETE_FILE_REQUEST))
        {
            char hashinfo[HASHINFO_LENGTH + 1];
            if (sscanf(buffer + 10, "%11s %21s %i", hashinfo, client_listen_ip, &client_listen_port) != 3)
            {
                send(clientSocket, "Failed to parse remaining information.", 39, 0);
                continue;
            }

            printf("-------Receive DELETE request from %s:%i with file %s-------\n", client_listen_ip, client_listen_port, hashinfo);
            mtx.lock();
            vector<pair<char *, int>> *iterator = &hashtable[hashinfo];
            if (iterator->empty())
            {
                send(clientSocket, "File not found", 15, 0);
                mtx.unlock();
                continue;
            }
            int size = iterator->size();
            for (int i = 0; i < size; i++)
            {
                if (!strcmp(client_listen_ip, (*iterator)[i].first) && client_listen_port == (*iterator)[i].second)
                {
                    if (size == 1)
                    {
                        (*iterator).clear();
                        if (listmap.size() == 1)
                            listmap.clear();
                        else
                        {
                            for (int i = 0; i < listmap.size(); i++)
                            {
                                if (!strcmp(listmap[i]->hashinfo, hashinfo))
                                {
                                    listmap.erase(listmap.begin() + i);
                                    break;
                                }
                            }
                        }
                    }
                    else
                        (*iterator).erase((*iterator).begin() + i);
                    break;
                }
            }
#ifdef DEBUG
            bool isEmpty = true;
            for (int i = 1; i <= listmap.size(); i++)
            {
                mapinfo *iter = listmap[i];
                printf("%i. Name: %s, filesize: %i, hashinfo: %s\n", i, iter->name, iter->hashinfo);
            }
            if (isEmpty)
                printf("Don's exist file in system.\n");
#endif
            send(clientSocket, "OK\n", 3, 0);
            mtx.unlock();
        }
        else
        {
            mtx.lock();
            send(clientSocket, "Unknown request!!", 18, 0);
            mtx.unlock();
        }
    }
}
