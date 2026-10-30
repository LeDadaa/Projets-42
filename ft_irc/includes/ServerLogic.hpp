/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLogic.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:23:12 by mbuisson          #+#    #+#             */
/*   Updated: 2026/04/10 12:00:16 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"


class ServerLogic
{
	public:
		ServerLogic(SendFn send_fn, const std::string& password);
		void	handleMessage(const Message &msg);
	
	private:

		std::map<int, Client*> clients;
		std::map<std::string, Channel> channels;
		
		SendFn _send;
		std::string	_password;
		
		void handlePass(const Message &msg);
		void handleNick(const Message &msg);
		void handleUser(const Message &msg);
		void handleJoin(const Message &msg);
		void handlePrivmsg(const Message &msg);
		void handleKick(const Message &msg);
		void handleInvite(const Message &msg);
		void handleTopic(const Message &msg);
		void handleMode(const Message &msg);
		void handleQuit(const Message &msg);
		void broadcast_mode(Channel &channel, Client *client,
								const std::string &channelName,
								const std::string &mode,
								const std::string &param);

		Client* getClient(int fd);
		Channel* getChannel(const std::string& name);
		void	send(int fd, const std::string& msg);
};