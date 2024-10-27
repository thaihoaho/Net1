#include "include/INFO.h"
#include "../file.h"

sockInfo listenSock;
mutex mtx;

unordered_map<string,vector<peerAddress*>> hashtable; // storage all peer address has a file with hashinfo

void handleRequest(SOCKET *clientSocket, char *buffer, char* client_ip, int client_port)
{
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
        char buffer[1024] = {0};
        
        if(send(*clientSocket,buffer,1024,0) == SOCKET_ERROR)
        {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            return;
        }
    }
    else if(!strcmp(requestID, DOWN_REQUEST)){
        int buffersize = sizeof(buffer) / sizeof(buffer[0]);
        // HASHINFO
        char hashinfo[HASHINFO_LENGTH + 1];
        strncpy(hashinfo, buffer + REQUEST_ID_LENGTH, HASHINFO_LENGTH);
        hashinfo[HASHINFO_LENGTH] = '\0';
        printf("Receive publish request with hashinfo: %s\n", hashinfo);

        vector<peerAddress*> listpeer = hashtable[hashinfo];
    }
}
int main(int argc, char *argv[])
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }

    listenSock = init((char *)SERVER_LISTEN_IP, SERVER_LISTEN_PORT);

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
        char* buffer = new char[1024];
        int bytesRead = recv(clientSocket, buffer, 1024, 0);

        // thread(handleRequest,&clientSocket, buffer,const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT).detach();
        handleRequest(&clientSocket,buffer,const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT);
    }

    return 0;
}