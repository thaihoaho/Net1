// this file use to handle all request from peer and tracker
#include "INFO.h"

// send data of sender
void sendData(SOCKET *sendedSocket, char* path, int pieceSize, int pieceOffset)
{
    char buffer[1024] = {0};
    ifstream readfile(path, ostream::binary);
    readfile.seekg(pieceOffset * pieceSize, std::ios::beg);
    readfile.read(buffer, pieceSize);
    buffer[pieceSize] = '\0';

    int bytesSent = send(*sendedSocket, buffer, pieceSize, 0);

    //TODO

    while (bytesSent <= 0)
    {
        printf("Send error");
        bytesSent = send(*sendedSocket, buffer, pieceSize, 0);
    }
    printf("Sent %i bytes from chunk %i \nbuffer : %s\n", bytesSent, pieceOffset, buffer);
}
// used in thread to listen all requests
void listenRequest()
{
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
       char buffer[1024] = {0};
        int bytesRead = recv(clientSocket, buffer, 1024, 0);
        std::string str(buffer);
        std::istringstream iss(str);

        std::string number1, name, number2, number3;

        std::getline(iss, number1, '-');
        std::getline(iss, name, '-');
        std::getline(iss, number2, '-');
        std::getline(iss, number3, '-');

        const char* requestID = number1.c_str();

        // receive request send file
        if (strcmp(requestID, DOWN_REQUEST) == 0)
        {
            printf("Thread of server receive %i bytes\n%s\n", bytesRead, buffer);
            const char* fileName  = name.c_str();
            int offset = stoi(number2);
            int partSize = stoi(number3); 
            thread(sendFileNthread,&clientSocket, fileName, offset, partSize).detach();
        }
        else if (strcmp(requestID, PING_REQUEST) == 0){
            printf("Thread of server receive %i bytes\n%s\n", bytesRead, buffer);
        }
    }
}

void sendFileNthread(SOCKET* clientSocket, const char *filePath, int offset, long required) {

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return;
    }

    file.seekg(offset*required, std::ios::beg);
    char buffer[512000];
    while ((file.read(buffer, 512000) || file.gcount() > 0) && required > 0) {
        std::streamsize bytesRead = file.gcount();
        int totalBytesSent = 0;

        while (totalBytesSent < bytesRead) {
            int bytesSent = send(*clientSocket, buffer + totalBytesSent, bytesRead - totalBytesSent, 0);
            if (bytesSent == SOCKET_ERROR) {
                std::cerr << "Error sending file chunk." << std::endl;
                return;
            }
            totalBytesSent += bytesSent;
        }
        std::cout << "Sent " << totalBytesSent << " bytes." << std::endl;
        required -= totalBytesSent;
    }
    file.close();
    closesocket(*clientSocket);
}