#include "../includes/Server.hpp"
#include "../includes/replies.hpp"
#include "../includes/utils.hpp"
#include "../includes/Message.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

void Server::processCommand(int fd, std::string command)
{
    std::vector<std::string> params = parseCommand(command);
    if(params.size() == 0)
        return;

    std::string cmd = params[0];
    for(size_t i = 0; i < cmd.size(); i++)
        cmd[i] = std::toupper(cmd[i]);

    if(_clients[fd]->isRegistered() == false)
    {
        if(cmd == "PASS")
            handlePass(fd, params);
        else if(cmd == "NICK")
            handleNick(fd, params);
        else if(cmd == "USER")
            handleUser(fd, params);
        else if(cmd == "QUIT")
            disconnectClient(fd);
        else
            sendReply(fd, ERR_NOTREGISTERED, "You have not registered");
        return;
    }

    if(cmd == "PASS")
        handlePass(fd, params);
    else if(cmd == "NICK")
        handleNick(fd, params);
    else if(cmd == "USER")
        handleUser(fd, params);
    else if(cmd == "QUIT")
        disconnectClient(fd);
    // else if(cmd == "JOIN")
    //     handleJoin(fd, params);
    // else if(cmd == "PRIVMSG")
    //     handlePrivmsg(fd, params);
    // else if(cmd == "KICK")
    //     handleKick(fd, params);
    // else if(cmd == "INVITE")
    //     handleInvite(fd, params);
    // else if(cmd == "TOPIC")
    //     handleTopic(fd, params);
    // else if(cmd == "MODE")
    //     handleMode(fd, params);
    else
        sendReply(fd, ERR_UNKNOWNCOMMAND, cmd + " :Unknown command");
}

void Server::handlePass(int fd, std::vector<std::string> params)    
{
    if(params.size() < 2)
    {
        sendReply(fd, ERR_NEEDMOREPARAMS, "PASS :Not enough parameters");
        return;
    }
    if(_clients[fd]->isRegistered() == true)
    {
        sendReply(fd, ERR_ALREADYREGISTRED , "You may not reregister");
        return;
    }
    if(this->_password == params[1])
        _clients[fd]->setPassOk(true);
}

void Server::handleNick(int fd, std::vector<std::string> params)
{
    if(params.size() < 2)
    {
        sendReply(fd, ERR_NONICKNAMEGIVEN, "No nickname given");
        return;
    }
    //check validité du Nickname.
    if(params[1].size() > 9 || isdigit(params[1][0]) )
    {
        sendReply(fd, ERR_ERRONEUSNICKNAME, "Erroneous nickname");
        return;
    }
    for(size_t i = 0 ; i < params[1].size(); i++)
    {
        if(params[1][i] == ' ')
        {
            sendReply(fd, ERR_ERRONEUSNICKNAME, "Erroneous nickname");
            return;
        }
    }
    std::map<int, Client*>::iterator it;
    for(it = _clients.begin(); it != _clients.end(); it++)
    {
        if(it->first != fd && it->second->getNickname() == params[1])
        {
            sendReply(fd, ERR_NICKNAMEINUSE, "Nickname is already in use");
            return;
        }
    }
    _clients[fd]->setNickname(params[1]);
    if(_clients[fd]->isPassOk() && _clients[fd]->getNickname().size() != 0 && _clients[fd]->getUsername().size() != 0)
    {
        _clients[fd]->setRegistered(true);
        std::string nick = _clients[fd]->getNickname();
        std::string user = _clients[fd]->getUsername();
        sendReply(fd, RPL_WELCOME, "Welcome to the IRC Network " + nick + "!" + user + "@localhost");
        sendReply(fd, RPL_YOURHOST, "Your host is " + this->_name + ", running version 1.0");
        sendReply(fd, RPL_CREATED, "This server was created today");
        sendReply(fd, RPL_MYINFO, this->_name + " 1.0 o itkol");
    }
}

void Server::handleUser(int fd, std::vector<std::string> params)
{
    if(params.size() < 5)
    {
        sendReply(fd, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
        return;
    }
    if(_clients[fd]->isRegistered() == true)
    {
        sendReply(fd, ERR_ALREADYREGISTRED, "You may not reregister");
        return;
    }
    if(_clients[fd]->isPassOk() == false)
    {
        sendReply(fd, ERR_PASSWDMISMATCH, "Password incorrect");
        return;
    }
    _clients[fd]->setUsername(params[1]);
    _clients[fd]->setRealname(params[4]);
    if(_clients[fd]->isPassOk() && _clients[fd]->getNickname().size() != 0 && _clients[fd]->getUsername().size() != 0)
    {
        _clients[fd]->setRegistered(true);
        std::string nick = _clients[fd]->getNickname();
        std::string user = _clients[fd]->getUsername();
        sendReply(fd, RPL_WELCOME, "Welcome to the IRC Network " + nick + "!" + user + "@localhost");
        sendReply(fd, RPL_YOURHOST, "Your host is " + this->_name + ", running version 1.0");
        sendReply(fd, RPL_CREATED, "This server was created today");
        sendReply(fd, RPL_MYINFO, this->_name + " 1.0 o itkol");
    }
}