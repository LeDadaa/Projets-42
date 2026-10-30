/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuisson <mbuisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:52:56 by damalca           #+#    #+#             */
/*   Updated: 2026/04/09 12:55:39 by mbuisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <iostream>
#include <set>
#include <vector>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>
#include <cstring>
#include <map>

#include "Channel.hpp"
#include "Client.hpp"
#include "Network.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Types.hpp"
#include "ServerLogic.hpp"
