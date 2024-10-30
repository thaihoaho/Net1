#include "include/INFO.h"
#include "../file.h"

int main(int argc, char *argv[])
{

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }

    // Listen thread to listen all request of another peer
    listenSock = init((char *)LISTEN_IP, LISTEN_PORT);
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
        else if (command == "fetch")
        {
            char request[1024] = {0};
            strcat(request,FETCH_REQUEST);
            strcat(request," ");
            strcat(request, LISTEN_IP);
            strcat(request," ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            sendRequest(const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT,request);
        }
        else if (command == "publish")
        {
            string name = (input.substr(index + 1, input.length() - index - 1) + '\0');
           
            char request[1024] = {0};
            strcat(request,PUBLISH_REQUEST);
            strcat(request," ");
            strcat(request, LISTEN_IP);
            strcat(request," ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            strcat(request, " ");
            strcat(request, name.c_str());

            sendRequest(const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request);
        }
        else if (command == "download"){
            char response[1024] = {0};
            strcat(response,DOWN_REQUEST);
            strcat(response,"0000000000");
            strcat(response," ");
            strcat(response, LISTEN_IP);
            strcat(response," ");
            strcat(response, to_string(LISTEN_PORT).c_str());
            
            sendRequest(const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, response);
            
        }
        else{
            printf("Command \"%s\" undefined!!\n",command.c_str());
        }
    }

    return 0;
}
