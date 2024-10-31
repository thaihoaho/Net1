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
        else if (command == "list"){
            list(0, "");
        }
        else if (command == "fetch")
        {
            char request[1024] = {0};
            strcat(request,FETCH_REQUEST);
            strcat(request," ");
            strcat(request, LISTEN_IP);
            strcat(request," ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            sendRequest(const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT,request, 0);
        }
        else if (command == "publish")
        {
            string name = (input.substr(index + 1, input.length() - index - 1));
            if(name.find(' ') != -1)
            {
                printf("command error!\n");
                continue;
            }
            char request[1024] = {0};
            long filesize = list(1, name);
            int pieces = piecesCount(filesize);
            strcat(request,PUBLISH_REQUEST);
            strcat(request," ");
            strcat(request, LISTEN_IP);
            strcat(request," ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            strcat(request," ");
            strcat(request, generateHashinfo(name, filesize, pieces, 512000));
            strcat(request," ");
            strcat(request, name.c_str());
            strcat(request," ");
            strcat(request, to_string(filesize).c_str());     
            strcat(request," ");
            strcat(request, to_string(pieces).c_str());
            strcat(request," ");
            strcat(request, to_string(512000).c_str());
            std::cout << request << std::endl;
            sendRequest(const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request, 0);
        }
        else if (command == "down"){
            string remainContent = (input.substr(index + 1, input.length() - index - 1) + '\0');     
            if(remainContent.find(' ') != -1)
            {
                printf("command error!\n");
                continue;
            }

            string s1= FETCH_REQUEST;
            string s2= LISTEN_IP;
            string s3= to_string(LISTEN_PORT);
            char s4[1024]= {0};
            strcpy(s4, const_cast<char*>((s1 + " " + s2 + " " + s3).c_str()));
            cout << s4 << endl;
            sendRequest(const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT,s4, 1, remainContent);
        }
        else{   
            printf("Command \"%s\" undefined!!\n",command.c_str());
        }
    }
    
    return 0;
}
