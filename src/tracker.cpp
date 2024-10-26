#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <thread>
#include <fstream>
#include <mutex>

#define LISTEN_IP "127.0.0.1"
#define LISTEN_PORT 70

struct sockInfo{
    SOCKET sock;
    sockaddr_in addr;
    sockInfo(SOCKET so, sockaddr_in ad){
        sock = so;
        addr = ad;
    }
};
#pragma comment(lib, "Ws2_32.lib") // include the Winsock 2 library during the linking phase
using namespace std;

SOCKET listenSocket = INVALID_SOCKET;
sockaddr_in listenAddr;
mutex mtx;

char *SHARE_BUFFER;

sockInfo createSockAddr(char *ip, int port, bool check = false)
{
    SOCKET sock;
    sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ; // TCP socket

    while (sock == INVALID_SOCKET)
    {
        printf("Create SOCKET error");
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP socket
    }

    addr.sin_family = AF_INET;              // IPv4
    inet_pton(AF_INET, ip, &addr.sin_addr); // IP address
    addr.sin_port = htons(port);            // Port number

    if (check)
    {
        if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
        { 
            std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
        }
    }
    return sockInfo(sock, addr);
}
// initialize
void init()
{
    sockInfo info = createSockAddr(LISTEN_IP, LISTEN_PORT);
    listenSocket = info.sock;
    listenAddr = info.addr;

    if (bind(listenSocket, (struct sockaddr *)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR)
    {
        cout << "Bind failed." << endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cerr << "Listen failed." << endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
}
// send data of sender
void sendData(SOCKET *sendedSocket, string path, int pieceSize, int pieceOffset)
{
    // use mutex to lock mem and file
    char *buffer = new char[pieceSize];
    ifstream readfile(path, ostream::binary);
    readfile.seekg(pieceOffset * pieceSize, std::ios::beg);
    readfile.read(buffer, pieceSize);

    int bytesSent = send(*sendedSocket, buffer, pieceSize, 0);

    while (bytesSent <= 0)
    {
        printf("Send error");
        bytesSent = send(*sendedSocket, buffer, pieceSize, 0);
    }

    cout << "Sent " << bytesSent << " bytes from chunk " << pieceOffset << endl;
    closesocket(*sendedSocket);
}
// used in thread to listen all requests
void listenRequest()
{
    while (true)
    {
        SOCKET clientSocket;
        sockaddr_in clientAddr;
        int addrLen = sizeof(listenAddr);
        clientSocket = accept(listenSocket, (struct sockaddr *)&listenAddr, &addrLen);

        if (clientSocket == INVALID_SOCKET)
        {
            cerr << "Accept failed." << endl;
            continue;
        }
        char buffer[1024] = {0};
        int bytesRead = recv(clientSocket, buffer, 1024, 0);

        // check request with first 10 bits
        char requestID[10];
        strncpy(requestID, buffer, 10);
        printf("%s\n", buffer);
        // receive request send file
        if (strcmp(requestID, "1111111111"))
        {
            int clientAddrlen = sizeof(clientAddr);
            getsockname(clientSocket, (struct sockaddr *)&clientAddr, &clientAddrlen);
            char *ip = inet_ntoa(clientAddr.sin_addr);
            int port = clientAddr.sin_port;
            sendData(&clientSocket, "files/aa.txt", 2, 0);
        }

        printf("Thread of server receive %i bytes\n%s\n", bytesRead, buffer);
    }
}

// receive data of reveiver
void waitData(SOCKET *receivedSocket, sockaddr_in addr, int pieceSize, int pieceOffset)
{
    lock_guard<mutex> lock(mtx);
    int addrLen = sizeof(addr);
    SOCKET sendedSocket = accept(*receivedSocket, (struct sockaddr *)&addr, &addrLen);

    if (sendedSocket == INVALID_SOCKET)
    {
        cerr << "Accept failed." << endl;
        return;
    }
    char *buffer = new char[pieceSize];
    int bytesRead = recv(sendedSocket, buffer, pieceSize, 0);

    strncpy(SHARE_BUFFER, buffer + pieceOffset * pieceSize, pieceSize);
    // check request
    printf("Thread of server receive %i bytes\n%s and SHARE_BUFFER %s\n", bytesRead, buffer, SHARE_BUFFER);
}
int main(int argc, char *argv[])
{

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }

    init();

    thread(listenRequest).detach();

    printf("Type \"help\" to get infomation\n");
    while (true)
    {
        string input;
        cin >> input;

        if (input == "help")
        {
            printf("help request");
        }
        else if (input == "fetch")
        {
            printf("fetch request");
        }
        else if (input == "sen")
        {
            printf("resquest send file");
            char *request = "1111111111";
            SHARE_BUFFER = new char[6];
            sockInfo a = createSockAddr(LISTEN_IP, 70, true), b = createSockAddr(LISTEN_IP, 60, true), c = createSockAddr(LISTEN_IP, 50, true);
            thread(send, a.sock, request, 10, 0).detach();
            thread(waitData, &a.sock, a.addr, 2, 0);
            // thread(send, b.sock, request, 10, 0).detach();
            // thread(waitData, &b.sock, a.addr, 2, 1);
            // thread(send, c.sock, request, 10, 0).detach();
            // thread(waitData, &c.sock, a.addr, 2, 2);
        }
    }

    // string op = argv[2];

    // // Perform the operation based on the operator
    // if (op == "-publish")
    // {
    //     cout << "-publish called" << endl;
    //     // Use this for file name
    //     if (argc < 3){
    //         cerr << "Error: Missing [file_name]. Usage: peer -publish [file_name]" << endl;
    //         exit(1);
    //     }
    //     //TODO: share file with other Peers.
    // } else if (op == "-fetch")
    // {
    //     cout << "-fetch called" << endl;
    //     if (argc < 3){
    //         cerr << "Error: Missing [file_name]. Usage: peer -fetch [file_name]" << endl;
    //         exit(1);
    //     }

    //     //TODO: Contact Tracker to get the list of peers sharing the file
    // }
    /* else if (op == "-----")
    {
        //TODO: Future implement
    }*/

    return 0;
}
