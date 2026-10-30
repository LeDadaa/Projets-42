/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLogic.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:46:56 by mbuisson          #+#    #+#             */
/*   Updated: 2026/04/22 14:56:39 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"

#include <cctype>
#include <ctime>

static bool sameNick(const std::string &lhs, const std::string &rhs) 
{
    if (lhs.size() != rhs.size())
        return false;
    for (size_t i = 0; i < lhs.size(); ++i) 
    {
        if (std::tolower(static_cast<unsigned char>(lhs[i]))
            != std::tolower(static_cast<unsigned char>(rhs[i])))
            return false;
    }
    return true;
}

static std::string botResponse(const std::string &content) 
{
    if (sameNick(content, "help"))
        return "commands: help, ping, time";
    if (sameNick(content, "ping"))
        return "pong";
    if (sameNick(content, "time")) 
    {
        std::time_t now = std::time(NULL);
        std::string value = std::ctime(&now);
        if (!value.empty() && value[value.size() - 1] == '\n')
            value.erase(value.size() - 1);
        return value;
    }
    if (content.empty())
        return "hello. type help for commands";
    return "i can answer: help, ping, time";
}

static bool isBotTrigger(const std::string &content, std::string &command) 
{
    if (content.compare(0, 4, "!bot") != 0)
        return false;
    if (content.size() == 4) 
    {
        command = "help";
        return true;
    }
    if (content[4] == ' ' && content.size() > 5) 
    {
        command = content.substr(5);
        return true;
    }
    if (content[4] == ' ') 
    {
        command.clear();
        return true;
    }
    return false;
}

static std::string botPrivmsg(const std::string &target, const std::string &reply) 
{
    return ":bot!bot@host PRIVMSG " + target + " :" + reply + "\r\n";
}

ServerLogic::ServerLogic(SendFn send_fn, const std::string& password) : _send(send_fn), _password(password)
{
}

void ServerLogic::send(int fd, const std::string& msg) 
{
	_send(fd, msg);
}
void	ServerLogic::handleMessage(const Message& msg)
{
	Message parsed = Parser::parse(msg.fd, msg.raw);

	if (clients.find(parsed.fd) == clients.end())
		clients[parsed.fd] = new Client(parsed.fd);

	if (parsed.command == "PASS")    
        handlePass(parsed);
	else if (parsed.command == "NICK")    
        handleNick(parsed);
	else if (parsed.command == "USER")    
        handleUser(parsed);
	else if (parsed.command == "JOIN")    
        handleJoin(parsed);
	else if (parsed.command == "PRIVMSG") 
        handlePrivmsg(parsed);
	else if (parsed.command == "KICK")    
        handleKick(parsed);
	else if (parsed.command == "INVITE")  
        handleInvite(parsed);
	else if (parsed.command == "TOPIC")   
        handleTopic(parsed);
	else if (parsed.command == "MODE")    
        handleMode(parsed);
	else if (parsed.command == "QUIT")    
        handleQuit(parsed);
	else if (parsed.command == "CAP")     
        send(parsed.fd, "CAP * LS :\r\n");
	else if (parsed.command == "PING")    
        send(parsed.fd, "PONG :" + (parsed.params.empty() ? "" : parsed.params[0]) + "\r\n");
	else
		send(parsed.fd, "421 " + parsed.command + " :Unknown command\r\n");
}

void ServerLogic::broadcast_mode(Channel &channel, Client *client, const std::string &channelName,
                                  const std::string &mode, const std::string &param) 
{
    std::string modeMsg = ":" + client->nickname + "!"
                        + client->username + "@host MODE "
                        + channelName + " " + mode;
    if (!param.empty())
        modeMsg += " " + param;
    modeMsg += "\r\n";

    std::vector<int> fds = channel.getMemberFds();
    for (size_t i = 0; i < fds.size(); i++)
        send(fds[i], modeMsg);
}

void ServerLogic::handlePass(const Message& msg)
{
	Client* client = clients[msg.fd];

	if (client->isRegistered())
    {
		send(msg.fd, "462 : you may not registered\r\n");
		return;
	}
	if (msg.params.empty())
    {
		send(msg.fd, "461 PASS : not enough params\r\n");
		return;
	}
	if (msg.params[0] != _password)
    {
		send(msg.fd, "464 : incorrect password\r\n");
		return;
	}
	client->authenticated = true;
}


void	ServerLogic::handleNick(const Message& msg)
{
	Client* client = clients[msg.fd];
	
	if (!client->authenticated)
    {
		send(msg.fd, "451 :You have not registered\r\n");
		return;
	}

	if (msg.params.empty())
    {
		send(msg.fd, "431 : No nickname given\r\n");
		return;
	}

	std::string newNick = msg.params[0];

    if (sameNick(newNick, "bot")) 
    {
        send(msg.fd, "433 * bot :Nickname is reserved\r\n");
        return;
    }
	
	for (size_t i = 0; i < newNick.size(); i++)
    {
		if (newNick[i] == ' ' || newNick[i] == '#'
			|| newNick[i] == ':' || newNick[i] == '@')
        {
				send(msg.fd, "435 : " + newNick + "unvalided nickname\r\n");
				return;
		}
	}
	
	for(std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
         if (sameNick(it->second->nickname, newNick) && it->first != msg.fd) 
         {
			send(msg.fd, "433 * " + newNick + " :Nickname is already in use\r\n");
			return;
		 }
	}
	std::string oldNick = client->nickname;
	client->nickname = newNick;

	if (!oldNick.empty() && client->registered)
		send(msg.fd, ":" + oldNick + " NICK " + newNick + "\r\n");

	else if (client->authenticated && !client->username.empty() && !client->registered) 
    {
		client->registered = true;
		send(msg.fd, "001 " + client->nickname + " :Welcome to the IRC server " 
					+ client->nickname + "!" + client->username + "@host\r\n");
		send(msg.fd, "002 " + client->nickname + " :Your host is ircserv\r\n");
		send(msg.fd, "003 " + client->nickname + " :This server was created today\r\n");
		send(msg.fd, "004 " + client->nickname + " :ircserv 1.0 o o\r\n");
	}
}

void ServerLogic::handleUser(const Message& msg){
	Client* client = clients[msg.fd];

	if (!client->authenticated) {
		send(msg.fd, "451 :You have not registered\r\n");
		return;
	}

	if (client->registered) {
		send(msg.fd, "462 :You may not reregister\r\n");
		return;
	}
	if (msg.params.size() < 4) {
		send(msg.fd, "461 USER :Not enough parameters\r\n");
		return;
	}

	client->username = msg.params[0];
	client->realname = msg.params[3];

	if (client->authenticated && !client->nickname.empty()) 
    {
		client->registered = true;
		send(msg.fd, "001 " + client->nickname + " :Welcome to the IRC server " 
					+ client->nickname + "!" + client->username + "@host\r\n");
		send(msg.fd, "002 " + client->nickname + " :Your host is ircserv\r\n");
		send(msg.fd, "003 " + client->nickname + " :This server was created today\r\n");
		send(msg.fd, "004 " + client->nickname + " :ircserv 1.0 o o\r\n");
	}
}

void ServerLogic::handleJoin(const Message &msg) 
{
    Client *client = clients[msg.fd];

    if (msg.params.empty()) 
    {
        send(msg.fd, "461 JOIN :Not enough parameters\r\n");
        return;
    }

    std::string channelName = msg.params[0];
    
    if (channelName == ":")
        return;

    if (!client->isRegistered()) 
    {
        send(msg.fd, "451 :You have not registered\r\n");
        return;
    }

    std::string key = msg.params.size() > 1 ? msg.params[1] : "";

    if (channelName[0] != '#') {
        send(msg.fd, "403 " + client->nickname + " " + channelName + " :No such channel\r\n");
        return;
    }

    if (channels.find(channelName) == channels.end())
        channels[channelName] = Channel(channelName);

    Channel &channel = channels[channelName];

    if (channel.isMember(msg.fd))
        return;

    if (channel.invite_only && !channel.isInvited(msg.fd)) 
    {
        send(msg.fd, "473 " + client->nickname + " " + channelName + " :Cannot join channel (+i)\r\n");
        return;
    }
    if (channel.hasKey() && channel.key != key) 
    {
        send(msg.fd, "475 " + client->nickname + " " + channelName + " :Cannot join channel (+k)\r\n");
        return;
    }
    if (channel.hasLimit() && (int)channel.members.size() >= channel.limit) 
    {
        send(msg.fd, "471 " + client->nickname + " " + channelName + " :Cannot join channel (+l)\r\n");
        return;
    }

    channel.addMember(client);

    std::string joinMsg = ":" + client->nickname + "!"  + "\r\n";
    std::vector<int> fds = channel.getMemberFds();
    for (size_t i = 0; i < fds.size(); i++)
        send(fds[i], joinMsg);

    if (channel.topic.empty())
        send(msg.fd, "331 " + client->nickname + " " + channelName + " :No topic is set\r\n");
    else
        send(msg.fd, "332 " + client->nickname + " " + channelName + " :" + channel.topic + "\r\n");

    std::string nameList = "";
    for (std::map<int, Client*>::iterator it = channel.members.begin();
         it != channel.members.end(); ++it) 
    {
        if (channel.isOperator(it->first))
            nameList += "@";
        nameList += it->second->nickname + " ";
    }
    send(msg.fd, "353 " + client->nickname + " = " + channelName + " :" + nameList + "\r\n");
    send(msg.fd, "366 " + client->nickname + " " + channelName + " :End of /NAMES list\r\n");
}

void ServerLogic::handlePrivmsg(const Message &msg) 
{
    Client *client = clients[msg.fd];

    if (!client->isRegistered()) 
    {
        send(msg.fd, "451 :You have not registered\r\n");
        return;
    }
    if (msg.params.size() < 2) 
    {
        send(msg.fd, "461 PRIVMSG :Not enough parameters\r\n");
        return;
    }

    std::string target  = msg.params[0];
    std::string content = msg.params[1];

	if (sameNick(target, "bot")) 
    {
		send(msg.fd, botPrivmsg(client->nickname, botResponse(content)));
		return;
	}

    if (target[0] == '#') 
    {
        if (channels.find(target) == channels.end()) 
        {
            send(msg.fd, "403 " + target + " :No such channel\r\n");
            return;
        }
        Channel &channel = channels[target];

        if (!channel.isMember(msg.fd)) 
        {
            send(msg.fd, "404 " + target + " :Cannot send to channel\r\n");
            return;
        }

        std::string privmsgStr = ":" + client->nickname + "!"
                               + client->username + "@host PRIVMSG "
                               + target + " :" + content + "\r\n";
        std::vector<int> fds = channel.getMemberFds(msg.fd);
        for (size_t i = 0; i < fds.size(); i++)
            send(fds[i], privmsgStr);

        std::string botCommand;
        if (isBotTrigger(content, botCommand)) 
        {
            std::string botMsg = botPrivmsg(target, botResponse(botCommand));
            std::vector<int> botFds = channel.getMemberFds();
            for (size_t i = 0; i < botFds.size(); i++)
                send(botFds[i], botMsg);
        }
    }
    else 
    {
        bool found = false;
        for (std::map<int, Client*>::iterator it = clients.begin();
             it != clients.end(); ++it) 
        {
            if (sameNick(it->second->nickname, target)) 
            {
                std::string privmsgStr = ":" + client->nickname + "!"
                                       + client->username + "@host PRIVMSG "
                                       + target + " :" + content + "\r\n";
                send(it->first, privmsgStr);
                found = true;
                break;
            }
        }
        if (!found)
            send(msg.fd, "401 " + target + " :No such nick\r\n");
    }
}

void ServerLogic::handleKick(const Message& msg)
{
	Client* client = clients[msg.fd];

	if (!client->registered)
	{
		send(msg.fd, "451 : you have not registered\r\n");
		return;
	}
	
	if (msg.params.size() < 2)
	{
		send(msg.fd, "461 : not enough params\r\n");
		return;
	}
	
	std::string channelName = msg.params[0];
	std::string targetNick = msg.params[1];
	std::string reason = msg.params.size() > 2 ? msg.params[2] : "NO reason";

	if (channels.find(channelName) == channels.end())
	{
		send(msg.fd, "403 : " + channelName + " : no such channel\r\n");
		return;
	}

	Channel &channel = channels[channelName];

	if (!channel.isMember(msg.fd))
	{
		send(msg.fd, "442 : " + channelName + " : you're not on that channel\r\n");
		return;
	}
	
	if (!channel.isOperator(msg.fd))
	{
		send(msg.fd, "482 : " + channelName + " : you're not channel operator\r\n");
		return;
	}
	
	int targetFd = -1;

	for(std::map<int, Client*>::iterator it = clients.begin();
			it != clients.end(); ++it)
    {
        if (sameNick(it->second->nickname, targetNick))
        {
			targetFd = it->first;
			break;
		}
	}
	
	if (targetFd == -1)
	{
		send(msg.fd, "401 : " + targetNick + " : no such nick\r\n");
		return;
	}

	if (!channel.isMember(targetFd))
	{
		send(msg.fd, "441 : " + targetNick + " " + channelName + " : they aren't on that channel\r\n");
		return;
	}
	
	std::string kickMsg = ":" + client->nickname + "!"
                        + client->username + "@host KICK "
                        + channelName + " " + targetNick + " :" + reason + "\r\n";
    std::vector<int> fds = channel.getMemberFds();
    for (size_t i = 0; i < fds.size(); i++)
        send(fds[i], kickMsg);

    channel.removeMember(targetFd);
}

void ServerLogic::handleInvite(const Message& msg)
{
	Client* client = clients[msg.fd];
	
	if (!client->isRegistered())
	{
		send(msg.fd, " 451 : you have not registered\r\n");
		return;
	}

	if (msg.params.size() < 2)
	{
		send(msg.fd,"461 PRIVMSG :Not enough parameters\r\n");
		return;
	}

	std::string targetNick = msg.params[0];
	std::string channelName = msg.params[1];
	
	
	if (channels.find(channelName) == channels.end())
	{
		send(msg.fd, "403 " + channelName + " :No such channel\r\n");
		return;
	}
	
	Channel &channel = channels[channelName];

	if (!channel.isMember(msg.fd))
	{
		send(msg.fd, "442 : " + channelName + " : you're not on that channel\r\n");
		return;
	}
	
	if (!channel.isOperator(msg.fd))
	{
		send(msg.fd, "482 : " + channelName + " : you're not channel operator\r\n");
		return;
	}

	int targetFd = -1;

	for(std::map<int, Client*>::iterator it = clients.begin();
			it != clients.end(); ++it)
    {
        if (sameNick(it->second->nickname, targetNick))
		{
			targetFd = it->first;
			break;
		}
	}
	if (targetFd == -1)
	{
		send(msg.fd, "401 : " + targetNick + " : no such nick\r\n");
		return;
	}

	channel.invited.insert(targetFd);
	send(msg.fd, "341 " + client->nickname + " " + targetNick + " " + channelName + "\r\n");
	send(targetFd, ":" + client->nickname + "!" + client->username 
				+ "@host INVITE " + targetNick + " :" + channelName + "\r\n");
}

void ServerLogic::handleTopic(const Message &msg) 
{
    Client *client = clients[msg.fd];

    if (!client->isRegistered()) 
    {
        send(msg.fd, "451 :You have not registered\r\n");
        return;
    }

    if (msg.params.empty()) 
    {
        send(msg.fd, "461 TOPIC :Not enough parameters\r\n");
        return;
    }

    std::string channelName = msg.params[0];

    if (channels.find(channelName) == channels.end()) {
        send(msg.fd, "403 " + channelName + " :No such channel\r\n");
        return;
    }

    Channel &channel = channels[channelName];

    if (!channel.isMember(msg.fd)) {
        send(msg.fd, "442 " + channelName + " :You're not on that channel\r\n");
        return;
    }
    if (msg.params.size() == 1) {
        if (channel.topic.empty())
            send(msg.fd, "331 " + client->nickname + " " + channelName + " :No topic is set\r\n");
        else
            send(msg.fd, "332 " + client->nickname + " " + channelName + " :" + channel.topic + "\r\n");
        return;
    }

    if (channel.topic_locked && !channel.isOperator(msg.fd)) {
        send(msg.fd, "482 " + channelName + " :You're not channel operator\r\n");
        return;
    }

    channel.topic = msg.params[1];

    std::string topicMsg = ":" + client->nickname + "!"
                         + client->username + "@host TOPIC "
                         + channelName + " :" + channel.topic + "\r\n";
    std::vector<int> fds = channel.getMemberFds();
    for (size_t i = 0; i < fds.size(); i++)
        send(fds[i], topicMsg);
}

void ServerLogic::handleMode(const Message &msg) 
{
    Client *client = clients[msg.fd];

    if (!client->isRegistered()) {
        send(msg.fd, "451 :You have not registered\r\n");
        return;
    }

    if (msg.params.size() < 2) {
        send(msg.fd, "461 MODE :Not enough parameters\r\n");
        return;
    }

    std::string channelName = msg.params[0];
    std::string modeStr     = msg.params[1];

    if (channels.find(channelName) == channels.end())
        return;

    Channel &channel = channels[channelName];

    if (!channel.isMember(msg.fd)) {
        send(msg.fd, "442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (!channel.isOperator(msg.fd)) {
        send(msg.fd, "482 " + channelName + " :You're not channel operator\r\n");
        return;
    }

    bool adding = true;
    size_t paramIndex = 2;

    for (size_t i = 0; i < modeStr.size(); i++) 
    {
        char c = modeStr[i];

        if (c == '+') 
        {
             adding = true;  
             continue; 
        }
        if (c == '-') 
        { 
            adding = false; 
            continue; 
        }

        if (c == 'i') 
        {
            channel.invite_only = adding;
            broadcast_mode(channel, client, channelName, (adding ? "+i" : "-i"), "");
        }

        else if (c == 't') 
        {
            channel.topic_locked = adding;
            broadcast_mode(channel, client, channelName, (adding ? "+t" : "-t"), "");
        }

        else if (c == 'k') 
        {
            if (adding) 
            {
                if (paramIndex >= msg.params.size()) 
                {
                    send(msg.fd, "461 MODE :Not enough parameters\r\n");
                    return;
                }
                channel.key = msg.params[paramIndex++];
                broadcast_mode(channel, client, channelName, "+k", channel.key);
            } 
            else 
            {
                channel.key = "";
                broadcast_mode(channel, client, channelName, "-k", "");
            }
        }

        else if (c == 'l') 
        {
            if (adding) 
            {
                if (paramIndex >= msg.params.size()) 
                {
                    send(msg.fd, "461 MODE :Not enough parameters\r\n");
                    return;
                }
                channel.limit = std::atoi(msg.params[paramIndex++].c_str());
                broadcast_mode(channel, client, channelName, "+l", msg.params[paramIndex - 1]);
            } 
            else 
            {
                channel.limit = 0;
                broadcast_mode(channel, client, channelName, "-l", "");
            }
        }

        else if (c == 'o') 
        {
            if (paramIndex >= msg.params.size()) 
            {
                send(msg.fd, "461 MODE :Not enough parameters\r\n");
                return;
            }

            std::string targetNick = msg.params[paramIndex++];
            int targetFd = -1;

            for (std::map<int, Client*>::iterator it = clients.begin();
                 it != clients.end(); ++it) 
            {
                if (sameNick(it->second->nickname, targetNick)) 
                {
                    targetFd = it->first;
                    break;
                }
            }
            if (targetFd == -1) 
            {
                send(msg.fd, "401 " + targetNick + " :No such nick\r\n");
                return;
            }
            if (!channel.isMember(targetFd)) 
            {
                send(msg.fd, "441 " + targetNick + " " + channelName
                           + " :They aren't on that channel\r\n");
                return;
            }
            if (adding)
                channel.operators.insert(targetFd);
            else
                channel.operators.erase(targetFd);

            broadcast_mode(channel, client, channelName,
                          (adding ? "+o" : "-o"), targetNick);
        }
        else
            send(msg.fd, "472 " + std::string(1, c) + " :is unknown mode char\r\n");
    }
}

void ServerLogic::handleQuit(const Message &msg) {
    Client *client = clients[msg.fd];

    std::string reason = msg.params.empty() ? "Client quit" : msg.params[0];

    std::string quitMsg = ":" + client->nickname + "!"
                        + client->username + "@host QUIT :" + reason + "\r\n";

    for (std::map<std::string, Channel>::iterator it = channels.begin();
         it != channels.end(); ++it) 
    {
        Channel &channel = it->second;
        if (channel.isMember(msg.fd)) 
        {
            std::vector<int> fds = channel.getMemberFds(msg.fd);
            for (size_t i = 0; i < fds.size(); i++)
                send(fds[i], quitMsg);
            channel.removeMember(msg.fd);
        }
    }
    send(msg.fd, "ERROR :Closing connection\r\n");
    delete clients[msg.fd];
    clients.erase(msg.fd);
}