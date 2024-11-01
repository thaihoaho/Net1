#include "include/INFO.h"

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }

    readSign();
                
    listenSocket = init((char *)SERVER_LISTEN_IP, SERVER_LISTEN_PORT);
    listenSocketBackup = init((char *)SERVER_LISTEN_IP_BACKUP, SERVER_LISTEN_PORT_BACKUP);

    //listenRequest(&listenSocket);
    thread lten(listenRequest, &listenSocket);
    lten.detach();
    thread ltenback(listenRequest, &listenSocketBackup);
    ltenback.detach();

    // Command-shell interpreter
    printf("Type \"help\" to get infomation\n");
    while (true)
    {
        string input;
        getline(cin, input);

        int index = input.find(' ');
        string command = input.substr(0, index);

        if (command == "help")
        {
            cout << "Available commands:" << endl;
            cout << "  discover         "<< "List all registered peers" << endl;
            cout << "  ping <ip:port>   "<< "Check connection between tracker and peer" << endl;
            cout << "  list             "<< "List all files have in system." << endl;
            cout << "  exit             "<< "Exit the tracker" << endl;
        }
        // Sua
        else if (command == "discover")
        {
            if (list_active_peer.empty())
                printf("The list of all active peer is empty.\n");
            else
            {
                printf("The list of all active peer.\n");
                for (int i = 0 ; i < list_active_peer.size();i++)
                {
                    if (!sendRequest(list_active_peer[i].first, list_active_peer[i].second, const_cast<char *>(PING_REQUEST), false))
                    {
                        list_active_peer.erase(list_active_peer.begin() + i);
                    }
                    else
                        printf("%s:%i\n", list_active_peer[i].first, list_active_peer[i].first);
                }
            }
        }
        else if (command == "ping")
        {
            char ip[16] = {0};
            int port = 0;
            if (sscanf(input.c_str() + 5, "%15[^:]:%i", ip, &port) != 2)
            {
                printf("Incorrect syntax. Type 'help' for available commands.");
                continue;
            };
#ifdef DEBUG
            printf("ip:%s , port:%i\n", ip, port);
#endif
            sendRequest(ip, port, const_cast<char *>(PING_REQUEST), true);
        }
        else if (command == "exit")
        {
            cout << "Exiting tracker..." << endl;
            running = false;
            break;
        }
        else if (command == "list")
        {
            bool isEmpty = true;
            for (int i = 0; i < listmap.size(); i++)
            {
                mapinfo *iter = listmap[i];
                printf("%i. Name: %s, filesize: %i, hashinfo: %s\n", i + 1, iter->name, iter->filesize, iter->hashinfo);
                isEmpty = false;
            }
            if (isEmpty)
                printf("No file exists in the system.\n");
        }
        else if (command == "listadv")
        {
            bool isEmpty = true;
            for (int i = 0; i < listmap.size(); i++)
            {
                mapinfo *iter = listmap[i];
                printf("%i. Name: %s, filesize: %i, hashinfo: %s\n", i + 1, iter->name, iter->filesize, iter->hashinfo);
                for (auto &iter : hashtable[iter->hashinfo])
                {
                    printf("    %s:%i\n", iter.first, iter.second);
                }
                isEmpty = false;
            }
            if (isEmpty)
                printf("No file exists in the system.\n");
        }
        else
        {
            cout << "Unknown command. Type 'help' for available commands." << endl;
        }
    }

    return 0;
}