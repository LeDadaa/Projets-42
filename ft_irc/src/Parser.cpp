/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 12:54:44 by mbuisson          #+#    #+#             */
/*   Updated: 2026/04/03 14:20:40 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"

Message Parser::parse(int fd, const std::string& line){
	Message msg;
	msg.fd = fd;
	msg.raw = line;

	std::string tmp = line;

	if (!tmp.empty() && tmp[0] == ':'){
		size_t space = tmp.find(' ');
		msg.prefix = tmp.substr(1, space -1);
		tmp = tmp.substr(space + 1);
	}
	
	size_t space = tmp.find(' ');
	if (space == std::string::npos){
		msg.command = tmp;
		for (size_t i = 0; i < msg.command.length(); i++)
			msg.command[i] = std::toupper(msg.command[i]);
		return msg;
	}
	msg.command = tmp.substr(0, space);
	for (size_t i = 0; i < msg.command.length(); i++)
		msg.command[i] = std::toupper(msg.command[i]);
	tmp = tmp.substr(space + 1);
	
	while (!tmp.empty())
	{
    	while (!tmp.empty() && tmp[0] == ' ')
        	tmp.erase(0, 1);

    	if (tmp.empty())
        	break;

    	if (tmp[0] == ':')
    	{
        	msg.params.push_back(tmp.substr(1));
        	break;
    	}

    	size_t next = tmp.find(' ');

    	if (next == std::string::npos)
    	{
        	msg.params.push_back(tmp);
        	break;
    	}

    	msg.params.push_back(tmp.substr(0, next));
    	tmp = tmp.substr(next + 1);
	}
	return msg;
}

