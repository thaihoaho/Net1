#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <thread>
#include <fstream>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib") // include the Winsock 2 library during the linking phase
using namespace std;

struct sockInfo{
    SOCKET sock;
    sockaddr_in addr;
    sockInfo() = default;
    sockInfo(SOCKET so, sockaddr_in ad){
        sock = so;
        addr = ad;
    }
};

sockInfo listenSock;
mutex mtx;

const char* LISTEN_IP = "127.0.0.1";
const int LISTEN_PORT = 8080;


sockInfo createSockAddr(char *ip, int port);
sockInfo init(char *ip, int port);


void waitData(SOCKET *socket);
void sendRequest(char *ip, int port, char *buffer);


void listenRequest();
void sendData(SOCKET *sendedSocket, char* path, int pieceSize, int pieceOffset);

