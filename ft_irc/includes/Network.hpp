/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:47:00 by damalca           #+#    #+#             */
/*   Updated: 2026/04/09 11:55:39 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Irc.h"

struct Message;

class Network 
{
    private:
        int _port;
        int _serverSocket;
        std::vector<struct pollfd> _pollFds;
        std::map<int, std::string> _clientBuffers;
        std::map<int, std::string> _clientWriteBuffers;
        void (*_msgHandler)(const Message&);

        void acceptNewClient();
        bool readClientEntry(size_t index);
        void sendClientData(size_t index);

    public:
        Network(int port);
        ~Network();

        void setMessageHandler(void (*handler)(const Message&));
        
        void init_network();
        void run_network();
        void queueData(int clientFd, const std::string& data);
};

