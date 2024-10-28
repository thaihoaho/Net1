// this file use to handle all command that user uses
#include "INFO.h"
// #include "../../file.h"
// receive data of reveiver
void waitData(SOCKET *socket)
{
    lock_guard<mutex> lock(mtx);
    char buffer[1024] = {0};
    int bytesRead = recv(*socket, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0)
    {
        cerr << "Receive failed: " << WSAGetLastError() << endl;
    }
    else
    {
        buffer[bytesRead] = '\0'; // Null-terminate the bufferr
        printf("Received %i bytes:\n%s\n", bytesRead, buffer);
    }

    
    closesocket(*socket);
}
void sendRequest(char *ip, int port, char *buffer)
{
    sockInfo socket = createSockAddr(ip, port);

    if (connect(socket.sock, (sockaddr *)&socket.addr, sizeof(socket.addr)) == SOCKET_ERROR)
    {       
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        return;
    }

    if (send(socket.sock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
    {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        return;
    }
    waitData(&socket.sock);
}

void receiveFileChunk(SOCKET sock, long offset, size_t chunkSize, int threadID, 
                        int num_of_peers, int filesize, std::vector<char>& sharedBuffer) {

    size_t pos = static_cast<size_t>(ceil(filesize / num_of_peers));
    std::vector<char> buffer(chunkSize);
    int bytesReceived;

    // Receive data in chunks
    size_t totalBytesReceived = 0;
    while ((bytesReceived = recv(sock, buffer.data(), chunkSize, 0)) > 0) {
        std::lock_guard<std::mutex> lock(bufferMutex);  // Lock for exclusive access

        // Check for boundaries to avoid buffer overflow
        if (pos * offset + totalBytesReceived + bytesReceived <= sharedBuffer.size()) {
            std::memcpy(sharedBuffer.data() + pos * offset + totalBytesReceived, buffer.data(), bytesReceived);
        }

        totalBytesReceived += bytesReceived;
        std::cout << "Thread " << threadID << " received " << bytesReceived << " bytes." << std::endl;
    }

    if (bytesReceived < 0) {
        std::cerr << "Thread " << threadID << " error receiving data." << std::endl;
    } else {
        std::cout << "Thread " << threadID << " finished receiving data." << std::endl;
    }
}


void sendRequestNthread(vector<pair<string, int>> v, char* name, int filesize){
    int num_of_peers =  static_cast<int>(v.size());
    std::vector<SOCKET> socks(num_of_peers);
    std::vector<sockaddr_in> sockAddr_ins(num_of_peers);
    std::cout << "Init ok" << std:: endl;
    for (int i = 0; i < num_of_peers; i++){
        socks[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socks[i] == INVALID_SOCKET) {
            std::cerr << "Socket creation failed for peer " << i << std::endl;
            WSACleanup();
            return;
        }
        sockAddr_ins[i].sin_family = AF_INET;
        sockAddr_ins[i].sin_port = htons(v[i].second);
        inet_pton(AF_INET, "127.0.0.1", &sockAddr_ins[i].sin_addr);
        
    }
    std::cout << "Socket creation ok" << std:: endl;

    for (int i = 0; i < num_of_peers; i++) {
        if (connect(socks[i], (sockaddr*)&sockAddr_ins[i], sizeof(sockAddr_ins[i])) == SOCKET_ERROR){
            std::cerr << "Connect failed for peer " << i << std::endl;
            closesocket(socks[i]);
            WSACleanup();
            return;
        }
    }
    std::cout << "Connections ok" << std:: endl;
    std::vector<char> sharedBuffer(filesize, ' ');
    std::vector<std::thread> threads;

    for (int i = 0; i < num_of_peers; i++) {
        threads.emplace_back(receiveFileChunk, socks[i], i, 512000, i + 1, num_of_peers, filesize, std::ref(sharedBuffer));
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join(); 
        }
    }
    std::cout << "OK";
    const char* fileName = "a2.jpg";
    std::ofstream outputFile(fileName, std::ios::binary);
    outputFile.write(sharedBuffer.data(), sharedBuffer.size()); 
    outputFile.close();
}