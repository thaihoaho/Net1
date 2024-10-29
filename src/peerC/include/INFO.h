#ifndef NETWORK_H
#define NETWORK_H
#include <sstream>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#include <mutex>
#include <vector>
#include <cmath>
#include <cstring>
#include <sstream>

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
// SOCKET
inline sockInfo listenSock;
inline mutex mtx;
inline mutex bufferMutex;
// ADDRESS 
inline const char *LISTEN_IP = "127.0.0.1";
inline const int LISTEN_PORT = 8080;

sockInfo createSockAddr(char *ip, int port);
sockInfo init(char *ip, int port);

void waitData(SOCKET *socket);
void sendRequest(char *ip, int port, char *buffer);

void listenRequest();
void sendData(SOCKET *sendedSocket, char *path, int pieceSize, int pieceOffset);

void sendRequestNthread(vector<pair<string, int>> v, char* name, int filesize);
void sendFileNthread(SOCKET *clientSocket, const char *filePath, int offset, long required);
#endif