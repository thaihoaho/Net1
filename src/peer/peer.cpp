#include "include/INFO.h"
#include "../file.h"

sockInfo listenSock;
mutex mtx;

const char* LISTEN_IP = "127.0.0.1";
const int LISTEN_PORT = 8080;

char *SHARE_BUFFER;

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
            thread(sendRequest,const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT,const_cast<char*>(FETCH_REQUEST)).detach();
        }
        else if (command == "publish")
        {
            string name = (input.substr(index + 1, input.length() - index - 1) + '\0');
            if(name.find(' ') != -1)
            {
                printf("command error!\n");
                continue;
            }
            //TODO
            //doc file path va lay thong tin cua file va them vao buffer gui len cho tracker

            thread(sendRequest,const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, const_cast<char*>(PUBLISH_REQUEST)).detach();
        }
        else if (command == "down"){
            string remainContent = PUBLISH_REQUEST +  (input.substr(index + 1, input.length() - index - 1) + '\0');          
            if(remainContent.find(' ') != -1)
            {
                printf("command error!\n");
                continue;
            }
            char* content = new char[remainContent.length() + 1];
            strcpy(content,remainContent.c_str());
            thread(sendRequest,const_cast<char*>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, content).detach();
        }
        else{
            printf("Command \"%s\" undefined!!\n",command.c_str());
        }
    }

    return 0;
}
