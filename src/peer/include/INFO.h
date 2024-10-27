#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct sockInfo
{
    SOCKET sock;
    sockaddr_in addr;
    sockInfo() = default;
    sockInfo(SOCKET so, sockaddr_in ad) : sock(so), addr(ad) {}
};
// SOCKET
extern sockInfo listenSock;
extern mutex mtx;
// ADDRESS 
extern const char *LISTEN_IP;
extern const int LISTEN_PORT;

sockInfo createSockAddr(char *ip, int port);
sockInfo init(char *ip, int port);

void waitData(SOCKET *socket);
void sendRequest(char *ip, int port, char *buffer);

void listenRequest();
void sendData(SOCKET *sendedSocket, char *path, int pieceSize, int pieceOffset);

#endif
