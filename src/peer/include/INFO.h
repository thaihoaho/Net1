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
inline sockInfo listenSock;
inline mutex mtx;
// ADDRESS 
inline const char *LISTEN_IP = "127.0.0.1";
inline const int LISTEN_PORT = 80808;

sockInfo createSockAddr(char *ip, int port);
sockInfo init(char *ip, int port);

void waitData(SOCKET *socket);
void sendRequest(char *ip, int port, char *buffer);

void listenRequest();
void sendData(SOCKET *sendedSocket, char *path, int pieceSize, int pieceOffset);

#endif
