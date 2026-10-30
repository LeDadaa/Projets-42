/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:46:55 by damalca           #+#    #+#             */
/*   Updated: 2026/04/09 12:21:24 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Irc.h"

class Client;

class Channel {
public:
    std::string            name;
    std::string            topic;
    std::string            key;
    int                    limit;
    bool                   invite_only;
    bool                   topic_locked;

    std::map<int, Client*> members;
    std::set<int>          operators;
    std::set<int>          invited;

    Channel();
    Channel(const std::string &name);

    void addMember(Client *c);
    void removeMember(int fd);
    bool isMember(int fd) const;
    bool isOperator(int fd) const;
    bool isInvited(int fd) const;

    bool hasKey()   const { return !key.empty(); }
    bool hasLimit() const { return limit > 0; }

    std::vector<int> getMemberFds(int exclude_fd = -1) const;
};