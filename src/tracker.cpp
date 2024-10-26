#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    std::string op = argv[2];

    // Perform the operation based on the operator
    if (op == "-discover")
    {
        std::cout << "-discover called" << std::endl;
        //TODO: Return list of peers
    } else if (op == "-ping")
    {
        std::cout << "-ping called" << std::endl;
        //TODO: Return ping status of all peers
    } 
    /* else if (op == "-----")
    {
        //TODO: Future implement
    }*/
    

    return 0;
}