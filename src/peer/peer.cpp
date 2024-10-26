#include "include/INFO.h"

sockInfo listenSock;
mutex mtx;

char *SHARE_BUFFER;
const char* LISTEN_IP = "127.0.0.1";
const int LISTEN_PORT = 8080;


int main(int argc, char *argv[])
{

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }

    listenSock = init((char *)LISTEN_IP, LISTEN_PORT);

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
            printf("resquest send file\n");
            char *request = (char *)"1111111111";
            thread(sendRequest,(char*)LISTEN_IP,8070,request).detach();
        }
    }

    return 0;
}
