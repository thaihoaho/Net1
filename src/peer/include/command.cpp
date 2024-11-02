// this file use to handle all command that user uses
#include "INFO.h"
// #include "../../file.h"
// receive data of reveiver
void waitData(SOCKET *socket, bool flag, char *buffer)
{
    int bytesRead = recv(*socket, buffer, 1023, 0);
    char requestID[11] = {0};
    sscanf(buffer, "%11s", requestID);


    if (bytesRead <= 0)
    {
        cerr << "Receive failed: " << WSAGetLastError() << endl;
    }
    else if(strcmp(requestID, FETCH_REQUEST) && bytesRead < 1024){
        cout << buffer << endl;
    }
    

    
    char name[100] = {0}, hashinfo[11] = {0};
    int filesize;
    if (!strcmp(requestID, FETCH_REQUEST))
    {
        int position = 11;
        int count = 1;
        while (position < bytesRead)
        {
            if(sscanf(buffer + position ,"%99s %10s %i",name, hashinfo, &filesize) != 3)
                break;
            position += strlen(name) + strlen(hashinfo) + strlen(to_string(filesize).c_str()) + 3;
            printf("%i. Name: %s, hashinfo: %s, filesize: %i bytes\n",count++, name, hashinfo,filesize);
        }
    }

    closesocket(*socket);
}
void sendRequest(char *ip, int port, char *buffer, int flag, string filename)
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
    char downBuffer[1024] = {0};
    waitData(&socket.sock, flag, downBuffer);
    if (flag == 3) {
        strcpy(buffer, downBuffer);
        return;
    } else
    if (flag == 1)
    {
        flag = 2;
        char *pos = downBuffer;
        char request[1024] = {0};
        char hashinfo[11] = {0};
        std::string fname;
        short start = 0, end = 0;
        bool s = true;

        while (*pos != '\0')
        {
            if (*pos == ' ')
            {
                if (s)
                {
                    start = pos - downBuffer + 1;
                    s = false;
                }
                else
                {
                    end = pos - downBuffer;
                    fname.assign(downBuffer + start, end - start);
                    s = true;
                    if (strcmp(filename.c_str(), fname.c_str()) == 0)
                    {
                        pos++;
                        break;
                    }

                    continue;
                }
            }
            pos++;
        }

        strncpy(hashinfo, pos, 10);
        strcat(request, "1010101010");
        strcat(request, " ");
        strcat(request, hashinfo);
        strcat(request, " ");
        strcat(request, LISTEN_IP);
        strcat(request, " ");
        strcat(request, to_string(LISTEN_PORT).c_str());
        mtx.lock();
        sendRequest(ip, port, request, flag);
        mtx.unlock();
    }
    else if (flag == 2)
    {
        flag = 0;
        // downBuffer = 1010101010 0000000000 video.mp4 20831306 40 512000 127.0.0.1 8080
        std::vector<std::pair<std::string, int>> v;
        int spaceCount = 0;
        const char *pos = downBuffer;
        short name_start = 0, name_end, size_start, size_end, num_of_peers_start, num_of_peers_end;
        while (spaceCount != 7)
        {
            if (*pos == ' ')
            {
                ++spaceCount;
                switch (spaceCount)
                {

                case 2:
                    name_start = pos - downBuffer + 1;
                    break;
                case 3:
                    name_end = pos - downBuffer;
                    size_start = pos - downBuffer + 1;
                    break;
                case 4:
                    size_end = pos - downBuffer;
                    break;
                case 6:
                    num_of_peers_start = pos - downBuffer + 1;
                    break;
                case 7:
                    num_of_peers_end = pos - downBuffer;
                    break;
                }
            }
            ++pos;
        }

        int name_len = name_end - name_start;
        int size_len = size_end - size_start;
        int n_len = num_of_peers_end - num_of_peers_start;

        std::string name(downBuffer + name_start, name_len);
        std::string size(downBuffer + size_start, size_len);
        std::string num_of_peers(downBuffer + num_of_peers_start, n_len);

        std::cout << "Name: " << name << std::endl;
        std::cout << "Size: " << size << std::endl;
        std::cout << "Num of peers: " << num_of_peers << std::endl;

        bool ip = 1;
        short ip_start = pos - downBuffer, ip_end, port_start, port_end;
        while (v.size() != stoi(std::string(num_of_peers)))
        {
            if (ip)
            {
                if (*pos == ' ')
                {
                    ip_end = pos - downBuffer;
                    port_start = pos - downBuffer + 1;
                    ip = 0;
                }
            }
            else
            {
                if (*pos == ' ')
                {
                    port_end = pos - downBuffer;

                    ip = 1;
                    int ip_size = ip_end - ip_start;
                    int port_size = port_end - port_start;

                    std::string Ip(downBuffer + ip_start, ip_size);
                    std::string port(downBuffer + port_start, port_size);
                    v.push_back(make_pair(Ip, stoi(port)));
                    ip_start = pos - downBuffer + 1;
                }
            }
            pos++;
        }
        
        const char *filepath = "files/";
        size_t length = std::strlen(filepath) + name.length() + 1;
        char *result = new char[length];
        std::strcpy(result, filepath);
        std::strcat(result, name.c_str());
        sendRequestNthread(v, const_cast<char *>(result), stoi(size));
    }
}

void receiveFileChunk(SOCKET sock, long offset, size_t chunkSize, int threadID,
                      int num_of_peers, int filesize, std::vector<char> &sharedBuffer, pair<string, int> peer)
{

    size_t pos = static_cast<size_t>(ceil(filesize / num_of_peers));
    std::vector<char> buffer(chunkSize);
    int bytesReceived;
    size_t totalBytesReceived = 0;
    while ((bytesReceived = recv(sock, buffer.data(), chunkSize, 0)) > 0)
    {
        std::lock_guard<std::mutex> lock(bufferMutex);

        if (pos * offset + totalBytesReceived + bytesReceived <= sharedBuffer.size())
        {
            std::memcpy(sharedBuffer.data() + pos * offset + totalBytesReceived, buffer.data(), bytesReceived);
        }
        totalBytesReceived += bytesReceived;
        std::cout << "Thread " << threadID << " received " << bytesReceived << " bytes from " << peer.first << ":" << peer.second  << std::endl;
    }

    if (bytesReceived < 0)
    {
        std::cerr << "Thread " << threadID << " error receiving data." << std::endl;
    }
    else
    {
        std::cout << "Thread " << threadID << " finished receiving data." << std::endl;
    }
}

void sendRequestNthread(vector<pair<string, int>> v, char *name, int filesize)
{
    int num_of_peers = static_cast<int>(v.size());
    std::vector<SOCKET> socks(num_of_peers);
    std::vector<sockaddr_in> sockAddr_ins(num_of_peers);
    std::cout << "Init ok" << std::endl;
    for (int i = 0; i < num_of_peers; i++)
    {
        socks[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socks[i] == INVALID_SOCKET)
        {
            std::cerr << "Socket creation failed for peer " << i << std::endl;
            WSACleanup();
            return;
        }
        sockAddr_ins[i].sin_family = AF_INET;
        sockAddr_ins[i].sin_port = htons(v[i].second);
        inet_pton(AF_INET, "127.0.0.1", &sockAddr_ins[i].sin_addr);
    }
    std::cout << "Socket creation ok" << std::endl;

    for (int i = 0; i < num_of_peers; i++)
    {
        if (connect(socks[i], (sockaddr *)&sockAddr_ins[i], sizeof(sockAddr_ins[i])) == SOCKET_ERROR)
        {
            std::cerr << "Connect failed for peer " << i << std::endl;
            closesocket(socks[i]);
            WSACleanup();
            return;
        }
    }
    std::cout << "Connections ok" << std::endl;
    std::vector<char> sharedBuffer(filesize, ' ');
    std::vector<std::thread> threads(num_of_peers);

    const char *filename = name;
    int offset;
    int partSize = ceil(filesize / num_of_peers);

    for (int i = 0; i < num_of_peers; i++)
    {
        offset = i;
        std::stringstream ss;
        ss << "1010101010" << "-" << name << "-" << offset << "-" << partSize;
        std::string result = ss.str();
        char *newBuffer = new char[result.size() + 1];
        std::strcpy(newBuffer, result.c_str());
        if (send(socks[i], newBuffer, strlen(newBuffer), 0) == SOCKET_ERROR)
        {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            return;
        }
    }

    for (int i = 0; i < num_of_peers; i++)
    {
        threads[i] = std::thread(receiveFileChunk, socks[i], i, 512000, i + 1, num_of_peers, filesize, std::ref(sharedBuffer), v[i]);
    }
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    
    string fileName = string(name);
    std::ofstream outputFile(fileName, std::ios::binary);
    outputFile.write(sharedBuffer.data(), sharedBuffer.size());
    outputFile.close();
}