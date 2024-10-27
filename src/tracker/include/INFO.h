#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#include <mutex>
#include <unordered_map>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct sockInfo
{
    SOCKET sock;
    sockaddr_in addr;
    sockInfo() = default;
    sockInfo(SOCKET so, sockaddr_in ad) : sock(so), addr(ad) {}
};

struct peerAddress{
    string ip;
    int port;
    peerAddress(char* i, int p){
        ip = i;
        port = p;
    }
};
// SOCKET
extern sockInfo listenSock;
extern mutex mtx;
// ADDRESS 
extern const char *SERVER_LISTEN_IP;
extern const int SERVER_LISTEN_PORT;

sockInfo createSockAddr(char *ip, int port);
sockInfo init(char *ip, int port);
#endif
