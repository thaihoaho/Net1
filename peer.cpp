#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    std::string op = argv[2];

    // Perform the operation based on the operator
    if (op == "-publish")
    {
        std::cout << "-publish called" << std::endl;
        // Use this for file name
        if (argc < 3){
            std::cerr << "Error: Missing [file_name]. Usage: peer -publish [file_name]" << std::endl;
            exit(1);
        }
        //TODO: share file with other Peers.
    } else if (op == "-fetch")
    {
        std::cout << "-fetch called" << std::endl;
        if (argc < 3){
            std::cerr << "Error: Missing [file_name]. Usage: peer -fetch [file_name]" << std::endl;
            exit(1);
        }

        //TODO: Contact Tracker to get the list of peers sharing the file
    }
    /* else if (op == "-----")
    {
        //TODO: Future implement
    }*/

    return 0;
}
