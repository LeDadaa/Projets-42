/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:47:21 by damalca           #+#    #+#             */
/*   Updated: 2026/04/22 15:00:16 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"

#include "../includes/Irc.h"

Network::Network(int port) : _port(port), _serverSocket(-1), _msgHandler(NULL) 
{
}

Network::~Network() 
{
    if (_serverSocket != -1)
        close(_serverSocket);
}

void Network::setMessageHandler(void (*handler)(const Message&))
{
    _msgHandler = handler;
}

void Network::acceptNewClient()
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientFd = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd == -1)
    {
        std::cerr << "Error: impossible to accept the new client" << std::endl;
        return;
    }
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error: impossible to set the new client into non-blocking mode" << std::endl;
        close(clientFd);
    }

    struct pollfd newClientPollFd;
    newClientPollFd.fd = clientFd;
    newClientPollFd.events = POLLIN;
    newClientPollFd.revents = 0;
    
    _pollFds.push_back(newClientPollFd);

    std::cout << "Connection established! New client logged in with fd : " << clientFd << std::endl;
}

bool Network::readClientEntry(size_t index)
{
    int     clientFd = _pollFds[index].fd;
    char    buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0)
    {
        std::cout << "Client on fd " << clientFd << " disconnected." << std::endl;
        close(clientFd);
        _clientBuffers.erase(clientFd);
        _pollFds.erase(_pollFds.begin() + index);
        return (true);
    }
    _clientBuffers[clientFd].append(buffer, bytesRead);
    size_t  pos;
    
    while ((pos = _clientBuffers[clientFd].find('\n')) != std::string::npos)
    {
        std::string command = _clientBuffers[clientFd].substr(0, pos);
        _clientBuffers[clientFd].erase(0, pos + 1);

        if (!command.empty() && command[command.length() - 1] == '\r')
            command.erase(command.length() - 1);
        if (_msgHandler)
        {
            Message msg = Parser::parse(clientFd, command);
            _msgHandler(msg);
        }
    }
    
    if (!_clientWriteBuffers[clientFd].empty())
        _pollFds[index].events |= POLLOUT;

    return (false);
}

void Network::init_network()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw (std::runtime_error("Error: socket creation failed"));
    
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw (std::runtime_error("Error: setsockopt failed"));

    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
        throw (std::runtime_error("Error: fcntl failed"));

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    if (bind(_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        throw (std::runtime_error("Error: bind failed. Is the port already used?"));

    if (listen(_serverSocket, SOMAXCONN) == -1)
        throw (std::runtime_error("Error: listen failed"));

    std::cout << "Server set on listen on port" << _port << "..." << std::endl;
}
void Network::queueData(int clientFd, const std::string& data)
{
    _clientWriteBuffers[clientFd] += data;

    for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        if (_pollFds[i].fd == clientFd)
        {
            _pollFds[i].events |= POLLOUT;
            break;
        }
    }
}

void Network::sendClientData(size_t index)
{
    int clientFd = _pollFds[index].fd;
    std::string& bufferOut = _clientWriteBuffers[clientFd];

    if (bufferOut.empty())
        return;

    ssize_t bytesSent = send(clientFd, bufferOut.c_str(), bufferOut.length(), 0);

    if (bytesSent > 0)
        bufferOut.erase(0, bytesSent);
    if (bufferOut.empty())
        _pollFds[index].events &= ~POLLOUT;
}

void Network::run_network()
{
    struct pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    serverPollFd.revents = 0;

    _pollFds.push_back(serverPollFd);

    std::cout << "Server is listening with poll()..." << std::endl;

    while (true)
    {
        int pollStatus = poll(&_pollFds[0], _pollFds.size(), -1);

        if (pollStatus == -1)
            throw std::runtime_error("Error: poll failed");

        for (size_t i = 0; i < _pollFds.size(); ++i)
        {
            if (_pollFds[i].revents & POLLIN)
            {
                if (_pollFds[i].fd == _serverSocket)
                    acceptNewClient();
                else
                {
                    if (readClientEntry(i) == true)
                    {
                        i--;
                        continue;
                    }
                }
            }
            if (_pollFds[i].revents & POLLOUT)
                sendClientData(i);
        }
    }
}
