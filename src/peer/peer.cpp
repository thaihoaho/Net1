#include "include/INFO.h"
#include "../file.h"

int main(int argc, char *argv[])
{
    bool logined = false;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed: " << endl;
        return 1;
    }
a:
    std::cout << "Type \"register\" or \"login\" or \"help\" to continue" << std::endl;
    while (true)
    {
        string input;
        getline(cin, input);

        int index = input.find(' ');
        string command = input.substr(0, index);
        if (command == "register")
        {
            std::string password1;
            std::string password2;
            std::cout << "Please enter password to register: " << std::endl;
            getline(cin, password1);
            std::cout << "Confirm password: " << std::endl;
            getline(cin, password2);

            if (password1 != password2)
            {
                std::cout << "Not matching. Please type \"register\" to try again" << std::endl;
                continue;
            }
            char request[1024] = {0};
            strcat(request, REGISTER_REQUEST);
            strcat(request, " ");
            strcat(request, LISTEN_IP);
            strcat(request, " ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            strcat(request, " ");
            strcat(request, password1.c_str());
            sendRequest(const_cast<char *>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request, 0);
        }
        else if (command == "login")
        {
            std::string password;
            std::cout << "Please enter password to login: " << std::endl;
            getline(cin, password);
            char request[1024] = {0};
            strcat(request, SIGNIN_REQUEST);
            strcat(request, " ");
            strcat(request, LISTEN_IP);
            strcat(request, " ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            strcat(request, " ");
            strcat(request, password.c_str());
            sendRequest(const_cast<char *>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request, 3);
            if (strcmp(request, std::string("OK").c_str()) == 0)
            {
                std::cout << "You're logged in! ";
                std::string path = "./files/";
                bool empty = 1;
                for (const auto &entry : fs::directory_iterator(path))
                {
                    if (fs::is_regular_file(entry.status()))
                    {
                        std::string name = entry.path().filename().string();
                        long filesize = (long)(fs::file_size(entry));
                        char request[1024] = {0};
                        int pieces = piecesCount(filesize);
                        strcat(request, PUBLISH_REQUEST);
                        strcat(request, " ");
                        strcat(request, LISTEN_IP);
                        strcat(request, " ");
                        strcat(request, to_string(LISTEN_PORT).c_str());
                        strcat(request, " ");
                        strcat(request, generateHashinfo(name, filesize));
                        strcat(request, " ");
                        strcat(request, name.c_str());
                        strcat(request, " ");
                        strcat(request, to_string(filesize).c_str());
                        strcat(request, " ");
                        strcat(request, to_string(pieces).c_str());
                        strcat(request, " ");
                        strcat(request, to_string(512000).c_str());
                        sendRequest(const_cast<char *>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request, 0);
                    }
                }
                break;
            }
        }
        else if (command == "help")
        {
            std::cout << "Available commands:" << std::endl;
            std::cout << " register              " << "Register your IP address and port with tracker with password" << std::endl;
            std::cout << " login                 " << "Enter the network with password" << std::endl;
            std::cout << std::endl
                      << "Note: You are currently not logged in. The available commands are limited to these options." << std::endl;
        }
        else
        {
            printf("Command \"%s\" undefined!!\n", command.c_str());
        }
    }
    // Listen thread to listen all request of another peer
    if (!logined)
    {
        listenSock = init((char *)LISTEN_IP, LISTEN_PORT);
        thread(listenRequest).detach();
        logined = true;
    }
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
            std::cout << "Available commands:" << std::endl;
            std::cout << " list                 " << "List all files in files/ that are available for sharing" << std::endl;
            std::cout << " fetch                " << "List all files that are available for downloading" << std::endl;
            std::cout << " publish <filename>   " << "Share a file with other peers (become a seeder)" << std::endl;
            std::cout << " down <filename>      " << "Download a file from other peer(s)" << std::endl;
            std::cout << " logout               " << "Leave the network" << std::endl;
        }
        else if (command == "list")
        {
            list(0, "");
        }
        else if (command == "fetch")
        {
            char request[1024] = {0};
            strcat(request, FETCH_REQUEST);
            strcat(request, " ");
            strcat(request, LISTEN_IP);
            strcat(request, " ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            sendRequest(const_cast<char *>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request, 0);
        }
        else if (command == "publish")
        {
            string name = (input.substr(index + 1, input.length() - index - 1));
            if (name.find(' ') != -1)
            {
                printf("command error!\n");
                continue;
            }
            char request[1024] = {0};
            long filesize = list(1, name);
            int pieces = piecesCount(filesize);
            strcat(request, PUBLISH_REQUEST);
            strcat(request, " ");
            strcat(request, LISTEN_IP);
            strcat(request, " ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            strcat(request, " ");
            strcat(request, generateHashinfo(name, filesize));
            strcat(request, " ");
            strcat(request, name.c_str());
            strcat(request, " ");
            strcat(request, to_string(filesize).c_str());
            strcat(request, " ");
            strcat(request, to_string(pieces).c_str());
            strcat(request, " ");
            strcat(request, to_string(512000).c_str());
            sendRequest(const_cast<char *>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request, 0);
        }
        else if (command == "down")
        {
            string remainContent = (input.substr(index + 1, input.length() - index - 1) + '\0');
            std::vector<std::string> fileNames;
            std::istringstream stream(remainContent);
            std::string fileName;

            while (stream >> fileName)
            {
                fileNames.push_back(fileName);
            }
            string s1 = FETCH_REQUEST;
            string s2 = LISTEN_IP;
            string s3 = to_string(LISTEN_PORT);
            char s4[1024] = {0};
            strcpy(s4, const_cast<char *>((s1 + " " + s2 + " " + s3).c_str()));
            std::vector<std::thread> threads(fileNames.size());
            for (int i = 0; i < fileNames.size(); i++)
            {
                threads[i] = std::thread(sendRequest, const_cast<char *>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, s4, 1, fileNames[i]);
            }

            for (auto &t : threads)
            {
                if (t.joinable())
                {
                    t.join();
                }
            }
        }
        else if (command == "logout")
        {
            char request[1024] = {0};
            strcat(request, SIGNOUT_REQUEST);
            strcat(request, " ");
            strcat(request, LISTEN_IP);
            strcat(request, " ");
            strcat(request, to_string(LISTEN_PORT).c_str());
            sendRequest(const_cast<char *>(SERVER_LISTEN_IP), SERVER_LISTEN_PORT, request, 0);
            goto a;
        }
        else
        {
            printf("Command \"%s\" undefined!!\n", command.c_str());
        }
    }

    return 0;
}
