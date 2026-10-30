/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:47:16 by damalca           #+#    #+#             */
/*   Updated: 2026/04/14 17:04:30 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Irc.h"

Network* g_network = NULL;
ServerLogic* g_logic = NULL;

void send_to_network(int fd, const std::string &msg)
{
    if (g_network != NULL)
        g_network->queueData(fd, msg + "\r\n");
}

void receive_message(const Message &msg)
{
    if (g_logic != NULL)
        g_logic->handleMessage(msg);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }

    int port = atoi(argv[1]);
    std::string password = argv[2];

    try
    {
        Network network(port);
        g_network = &network;
        
        network.setMessageHandler(receive_message);
        network.init_network();

        ServerLogic server_logic(send_to_network, password);
        g_logic = &server_logic;

        std::cout << "Starting IRC server on port " << port << "..." << std::endl;
        network.run_network();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return (1);
    }

    return (0);
}

