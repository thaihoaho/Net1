#ifndef NETWORK_H
#define NETWORK_H
#include "../../file.h"

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct sockInfo
{
    SOCKET sock;
    sockaddr_in addr;
    sockInfo() = default;
    sockInfo(SOCKET so, sockaddr_in ad) : sock(so), addr(ad) {}
};

struct mapinfo
{
    char *hashinfo;
    char *name;
    int filesize, piececount, piecesize;
    mapinfo(char *hash, char *na, int fs, int pc, int ps)
        : hashinfo(hash), name(na), filesize(fs), piececount(pc), piecesize(ps) {}
    ~mapinfo() {
        delete[] hashinfo; 
        delete[] name;      
    }
};
inline vector<mapinfo*> listmap;
inline vector<pair<char*,int>> list_active_peer;
inline unordered_map<string, vector<pair<char*, int>>> hashtable; // storage all peer address has a file with hashinfo
inline bool running = true;
inline char ping_request[33] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0',0};
// SOCKET
inline sockInfo listenSocket;
inline sockInfo listenSocketBackup;
inline mutex mtx;

sockInfo createSockAddr(char *ip, int port);
sockInfo init(char *ip, int port);

void listenRequest(sockInfo *listenSock);
void sendData(SOCKET *sendedSocket, char *path, int pieceSize, int pieceOffset);

void sendRequest(char *ip, int port, char *buffer);
#endif
    