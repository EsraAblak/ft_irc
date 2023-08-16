#include "includes/HandleMessage.hpp"

std::map<std::string, ICommand *> HandleMessage::getCommandMap(){
	return (_commandMap);
}

void HandleMessage::processNotAuthenticated(Server &server, Client &client)
{
    if (!client.getMap().empty())
	{
		if (client.getMap().front().first == "PASS")
			_commandMap.insert(std::make_pair("PASS",new PASS()));
		else
			server.messageToClient(client.getClientFd(),"Error: you can only send PASS\n");
    }
}

void HandleMessage::processAuthenticate(Server &server, Client &client)
{
	if (!client.getMap().empty())
	{
		if (client.getMap().front().first != "NICK"
			&& client.getMap().front().first != "USER")
		{
			server.messageToClient(client.getClientFd(),
					"Error: you can only send NICK or USER\n");
		}
		else
		{
			client.setMapSecondEnd();
			if (client.getMap().front().first == "NICK")
			{
				checks::checkNick(client.getMap().front().second);
				client.setNickname(client.getMap().front().second);
			}
			else
				client.setUsername(client.getMap().front().second);
		}
	}
	if (!(client.getNickname() == "" || client.getUsername() == ""))
	{
		client.setAuthStatus(REGISTERED);
		std::cout << "registered" << std::endl;
	}
}

void HandleMessage::processRegistered(Server &server, Client &client)
{
    (void) server;
	(void)client;
	return ;
}

void HandleMessage::clientMsgProcess(Server &server, Client &client){
    if (client.getAuthStatus() == NOTAUTHENTICATED)
    	processNotAuthenticated(server,client);
    else if (client.getAuthStatus() == AUTHENTICATE)
    	processAuthenticate(server,client);
    else
    	processRegistered(server,client);
}

int HandleMessage::handleMsg(Server &server, Client &client, std::string msg){
    if (msg.size() > 512)
	{
		std::cout << "512" << std::endl; //kendisi handleliyo
		client.getNums().handleNumeric("417", "ERR_INPUTTOOLONG", client, server);
		msg[511] = '\r';
		msg[512] = '\n';
	}

	if (msg == "")
		return 0;

	size_t findPos = msg.find(' ');
	std::string first;
	std::string second;

    if (findPos == std::string::npos){
		first = msg;
		second = "";
		if (client.getAuthStatus() == NOTAUTHENTICATED && first != "PASS"){
			server.messageToClient(client.getClientFd(),"Error: you can only send PASS\n");
			return (0);
		}else if (client.getAuthStatus() == AUTHENTICATE && (first != "NICK" || first != "USER")){
			server.messageToClient(client.getClientFd(),"Error: you can only send NICK or USER\n");
			return (0);
		}
	}

    first = msg.substr(0, findPos);
	second = msg.substr(findPos + 1);
    size_t f_pos = 0;
    if (second[0] != ':'){
        f_pos = second.find(' ');
        if (f_pos != std::string::npos){
            second = second.substr(0,f_pos);
        }
    }else{
		second = second.substr(f_pos+1);
	}
    client.setClientMessage(first, second);
    return (1);
}