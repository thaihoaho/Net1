#include "INFO.h"

// create socket, addr
sockInfo createSockAddr(char *ip, int port)
{
    SOCKET sock;
    sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP socket

    while (sock == INVALID_SOCKET)
    {
        printf("Create SOCKET error");
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP socket
    }

    addr.sin_family = AF_INET;              // IPv4
    inet_pton(AF_INET, ip, &addr.sin_addr); // IP address
    addr.sin_port = htons(port);            // Port number
    return sockInfo(sock, addr);
}
// initialize for listen
sockInfo init(char *ip, int port)
{
    sockInfo info = createSockAddr((char *)ip, port);
    if (bind(info.sock, (struct sockaddr *)&info.addr, sizeof(info.addr)) == SOCKET_ERROR)
    {
        cout << "Bind failed." << endl;
        closesocket(info.sock);
        WSACleanup();
    }
    if (listen(info.sock, 10) == SOCKET_ERROR)
    {
        cerr << "Listen failed." << endl;
        closesocket(info.sock);
        WSACleanup();
    }
    return info;
}

void readSign(){
    std::ifstream infile("sign.txt");
    if (infile.is_open())
    {
        std::string line= "";
        std::string ip;
        std::string port;
        std::string password;
        while (getline(infile, line)) {
            size_t index = line.find(' ');
            if (index != std::string::npos)
            {
                ip = line.substr(0, index);
                port = line.substr(index + 1);

            } else {
                password = line;
                peerinfo* k = new peerinfo(const_cast<char*>(ip.c_str()), stoi(port), const_cast<char*>(password.c_str()));
                list_peer_info.push_back(k);
            }
        }
        infile.close();
    }
    else
    {
        std::cerr << "sign.txt cannot be opened\n";
    }

}