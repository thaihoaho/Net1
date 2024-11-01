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
inline vector<mapinfo*> listmap; // storage information of files in system
struct peerinfo{
    char* ip;
    int port;
    char* password;
    peerinfo(char* i, int p, char* pass){
        ip = strdup(i);
        port = p;
        password = strdup(pass);
    }
};
inline vector<peerinfo*> list_peer_info; // storage all registed peer information
inline vector<pair<char*,int>> list_active_peer; // storage all active peer address
inline unordered_map<string, vector<pair<char*, int>>> hashtable; // storage all peer address has a file with hashinfo
inline bool running = true;
inline char ping_request[33] = "000000000000000000000000000000\0";
// SOCKET
inline sockInfo listenSocket;
inline sockInfo listenSocketBackup;
inline mutex mtx;

sockInfo createSockAddr(char *ip, int port);
sockInfo init(char *ip, int port);

void listenRequest(sockInfo *listenSock);
void sendData(SOCKET *sendedSocket, char *path, int pieceSize, int pieceOffset);

bool sendRequest(char *ip, int port, char *buffer, bool pri);

void readSign();
#endif
    