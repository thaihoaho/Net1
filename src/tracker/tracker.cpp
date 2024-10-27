#include "include/INFO.h"

sockInfo listenSock;
mutex mtx;

unordered_map<string,pair<string, int>> hashtable; // storage all peer address has a file with hashinfo

void handleRequest(SOCKET *clientSocket, char *buffer, char* client_ip, int client_port)
{
   
}
int main(int argc, char *argv[])
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }

    listenSock = init((char *)SERVER_LISTEN_IP, SERVER_LISTEN_PORT);
    thread(listenRequest).detach();

    // Command-shell interpreter
    printf("Type \"help\" to get infomation\n");
    while (true)
    {
        string input;
        getline(cin,input);

        int index = input.find(' ');
        string command = input.substr(0, index);
        
        if (command == "help")
        {
            printf("Help request\n");
        }
        // TODO
    }

    return 0;
}