#include "Server.hpp"

class Channel;
class Server;

class Client
{
    private:
        int _fd;
        std::string _ip;
        int _port;

        std::string _recvBuffer;
        std::string _sendBuffer;

        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _hostname;

        bool _passOk;
        bool _nickOk;
        bool _userOk;
        bool _registered;

        std::set<std::string> _channels;
        
        time_t _lastActivity;

    public:
        Client(int fd, const std::string& ip, int port);
        ~Client();

        int getFd() const;
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        const std::string& getRealname() const;
        const std::string& getHostname() const;
        const std::string& getPrefix() const; // nick!user@host
        const std::string& getSendBuffer() const;
        bool isRegistered() const;

        void setNickname(const std::string& nick);
        void setUsername(const std::string& user);
        void setRealname(const std::string& real);
        void setPassOk(bool ok);
        void setNickOk(bool ok);
        void setUserOk(bool ok);

        void appendToRecvBuffer(const std::string& data);
        void appendToSendBuffer(const std::string& data);
        bool hasCompleteCommand() const;
        std::string extractCommand();
        void clearSentData(size_t bytes);

        std::string makeReply(const std::string& code, const std::string& target, const std::string& message);
        void        sendMessage(const std::string& message);

        void addChannel(const std::string& channel);
        void removeChannel(const std::string& channel);
        bool isInChannel(const std::string& channel) const;

        void checkRegistration();
};