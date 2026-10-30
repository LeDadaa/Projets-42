/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:47:03 by damalca           #+#    #+#             */
/*   Updated: 2026/04/02 14:45:35 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Irc.h"

class Server
{
	private:
		int			_port;
		std::string	_password;

	public:
		Server(int port, std::string password);
		~Server();

		void	run_server();
};