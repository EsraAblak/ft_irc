#ifndef HANDLEMESSAGE_HPP
#define HANDLEMESSAGE_HPP

#include "Server.hpp"
#include "ICommand.hpp"

class Server;
class Client;
class ICommand;

class HandleMessage{

    private:
    // ICommand'ekle
    std::map<std::string, ICommand *> _commandMap;

    public:
    int handleMsg(Server &server, Client *client, std::string msg);
    void clientMsgProcess(Server &server, Client *client);
    void processNotAuthenticated();
    void processAuthenticate();
    void processRegistered();
    std::map<std::string, ICommand *> getCommandMap();
    ICommand * getCommand(std::string command);
    int checkAuthCommand(Server &server,Client *client);
    void removeParams(Client *client);
};
#endif
