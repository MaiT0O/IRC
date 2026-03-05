#include "../includes/Server.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

Server::Server(int _port, std::string _password)
{
    this->_port = _port;
    this->_password = _password;
    initSocket();
}

Server::~Server()
{

}
void Server::run()
{

}

void Server::stop()
{
    
}

void Server::initSocket()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
    this->_fdserver = serverSocket;
    int opt = 1;
    if(setsockopt(_fdserver, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) == -1))
        throw std::runtime_error("Failed to set socker options");
    if(fcntl(_fdserver, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set non-blocking mode");
    sockaddr_in serverAddr;
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(_port);
      serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(_fdserver, (sockaddr*)&serverAddr, sizeof(serverAddr)))
}