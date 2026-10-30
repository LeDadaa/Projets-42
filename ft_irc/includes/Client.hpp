/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:46:57 by damalca           #+#    #+#             */
/*   Updated: 2026/04/09 12:21:14 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Irc.h"


class Client
{
	public:
		int fd;
		std::string nickname;
		std::string username;
		std::string realname;
		std::string buffer;
		bool authenticated;
		bool registered;
		std::string password_attempt;

		Client(int fd);
		bool isRegistered() const;
		~Client();
};