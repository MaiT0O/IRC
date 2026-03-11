#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if(port <= 0 || port > 65535)
    {
        std::cout << "Error: invalid port number" << std::endl;
        return 1;
    }

    std::string password = argv[2];
    if(password.empty())
    {
        std::cout << "Error: password cannot be empty" << std::endl;
        return 1;
    }

    try
    {
        Server server(port, password);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}