#include "Server.hpp"

class Client;
class Server;

class Channel
{
    private:
        std::string _name; // Nom du channel (#channel)
        std::string _topic; // Sujet du channel
        std::string _topicSetter; // Qui a défini le topic
        time_t _topicTime; // Quand le topic a été défini
        std::string _key; // Mot de passe (mode +k)
        size_t _userLimit; // Limite d'utilisateurs (mode +l)
        // Membres
        std::map<int, Client*> _members; // fd -> Client*
        std::set<int> _operators; // fds des opérateurs
        std::set<int> _invited; // fds des invités (mode +i)
        // Modes
        bool _inviteOnly; // +i : invitation seulement
        bool _topicRestricted; // +t : topic réservé aux ops
        bool _hasKey; // +k : mot de passe requis
        bool _hasLimit; // +l : limite d'utilisateurs

    public:
        Channel(const std::string& name);
        ~Channel();
        // Getters
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getTopicSetter() const;
        const time_t& getTopicTime() const;
        const std::string& getKey() const;
        const size_t& getUserLimit() const;
        const size_t& getMemberCount() const;
        std::string getModeString() const; // Retourne "+itk" etc.
        // Setters
        void setTopic(const std::string& topic, const std::string& setter);
        void setKey(const std::string& key);
        void setUserLimit(size_t limit);
        void setInviteOnly(bool value);
        void setTopicRestricted(bool value);
        // Gestion des membres
        void addMember(Client* client);
        void removeMember(Client* client);
        bool isMember(Client* client) const;
        std::vector<Client*> getMembers() const;
        std::string getMemberList() const; // Liste des nicks pour NAMES
        // Gestion des opérateurs
        void addOperator(int fd);
        void removeOperator(int fd);
        bool isOperator(int fd) const;
        bool isOperator(Client* client) const;
        // Gestion des invitations
        void addInvite(int fd);
        void removeInvite(int fd);
        bool isInvited(int fd) const;
        // Vérifications
        bool canJoin(Client* client, const std::string& key) const;
        bool canSpeak(Client* client) const;
        bool canChangeTopic(Client* client) const;
        // Broadcast
        void broadcast(const std::string& message, Client* exclude = NULL);
};