#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <fstream>
#include <cmath>
#include <filesystem>
#pragma comment(lib, "ws2_32.lib")
namespace fs = std::filesystem;
#define BUFFER_SIZE 1024
void get_share_info(){
    std::string path = "shared_folder";
    for (const auto& entry : fs::directory_iterator(path)) {
        // Skip "desktop.ini"
        if (entry.path().filename() != "desktop.ini") {
            // Get the file size
            try {
                auto size = fs::file_size(entry.path());
                std::cout << entry.path().filename() << " - " << size << " bytes" << std::endl;
            }
            catch (const fs::filesystem_error& e) {
                // Handle error (e.g., if entry is a directory, not a file)
                std::cerr << "Error accessing file size: " << e.what() << std::endl;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc > 1){
    std::string op = argv[2];

    // Perform the operation based on the operator
    if (op == "-publish")
    {
        std::cout << "-publish called" << std::endl;
        
        if (argc < 3){
            std::cerr << "Error: Missing [file_name]. Usage: peer -publish  [file_name]" << std::endl;
            exit(1);
        }

        std::string filename = argv[3];

        //TODO: share file with other Peers.
    } else if (op == "-fetch")
    {
        std::cout << "-fetch called" << std::endl;
        if (argc < 3){
            std::cerr << "Error: Missing [file_name]. Usage: peer -fetch [file_name]" << std::endl;
            exit(1);
        }
        std::string filename = argv[3];

    } 
    }
    get_share_info();
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr) == SOCKET_ERROR)) {
        std::cerr << "Failed to connect to server tracker" << std::endl;
        return 1;
    }
    
    std::string message = "Hello, Server! This is a test string."; 
    int bytesSent = send(sock, message.c_str(), message.size(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed. Error Code: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }


    return 0;
}
