/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 15:26:33 by mbuisson          #+#    #+#             */
/*   Updated: 2026/04/03 12:03:52 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"

Client::Client(int fd) : fd(fd), authenticated(false), registered(false){}

bool Client::isRegistered() const{
	return authenticated && !nickname.empty() && !username.empty();
}

Client::~Client(){}