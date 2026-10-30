/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 15:22:52 by mbuisson          #+#    #+#             */
/*   Updated: 2026/04/03 14:20:11 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"

struct Message
{
	int fd;
	std::string raw;
	std::string prefix;
	std::string command;
	std::vector<std::string> params; 
};