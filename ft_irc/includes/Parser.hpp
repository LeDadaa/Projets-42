/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 13:01:37 by mbuisson          #+#    #+#             */
/*   Updated: 2026/04/03 13:03:58 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"

class Parser
{
	public:
		static Message parse(int fd, const std::string& line);
};