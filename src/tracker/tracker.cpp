#include "include/INFO.h"

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }

    listenSock = init((char *)SERVER_LISTEN_IP, SERVER_LISTEN_PORT);
    // thread(listenRequest).detach();
    listenRequest();

    // Command-shell interpreter
    printf("Type \"help\" to get infomation\n");
    while (true)
    {
        string input;
        cout << ">>";
        getline(cin,input);

        int index = input.find(' ');
        string command = input.substr(0, index);
        
        if (command == "help")
        {
            printf("Help request\n");
        }
        else if(command == "ping"){
            
        }
    }

    return 0;
}