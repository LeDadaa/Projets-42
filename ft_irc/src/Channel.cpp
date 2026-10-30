/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 15:29:49 by mbuisson          #+#    #+#             */
/*   Updated: 2026/04/09 13:00:18 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.h"

Channel::Channel(const std::string& name)
	: name(name), limit(0), invite_only(false),
	  topic_locked(false){}

Channel::Channel() : limit(0), invite_only(false), topic_locked(false) {}

void Channel::addMember(Client *c){
	std::pair<std::map<int, Client*>::iterator, bool> result =
		members.insert(std::make_pair(c->fd, c));

	if (result.second) {
		if (members.size() == 1)
			operators.insert(c->fd);
		invited.erase(c->fd);
	}
	else {
	}
}

void Channel::removeMember(int fd){
	members.erase(fd);
	operators.erase(fd);
}

bool Channel::isMember(int fd) const{
	return (members.find(fd) != members.end());
}

bool Channel::isInvited(int fd) const{
	return invited.count(fd) > 0;
}

bool Channel::isOperator(int fd) const {
	return operators.count(fd) > 0;
}

std::vector<int> Channel::getMemberFds(int exclude_fd) const {
	std::vector<int> fds;
	for (std::map<int, Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (it->first != exclude_fd)
			fds.push_back(it->first);
	}
	return fds;
}