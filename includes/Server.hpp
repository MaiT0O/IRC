#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <poll.h>
#include <netinet/in.h>
#include <stdexcept>
#include <time.h>
#include <ctime>
#include <sys/socket.h>
#include "Message.hpp"
#include "Client.hpp"

class Client;
class Channel;

class Server
{
    private:
        int         _port;
        std::string _password;
        int         _fdserver;
        std::string _name;

        std::vector<pollfd> _pollfd;
        std::map <int, Client*> _clients;
        std::map <std::string, Channel*> _channels;


        void initSocket();
        void acceptClient();
        void handleClient(int fd);
        void disconnectClient(int fd);
        
    public:
        Server(int _port, std::string _password);
        ~Server();

        void run();
        void stop();

};
#endif