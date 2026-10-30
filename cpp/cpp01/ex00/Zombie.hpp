/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:20:08 by damalca           #+#    #+#             */
/*   Updated: 2025/11/26 04:32:32 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>

class Zombie {
	private:
		std::string _name;
		
	public:
		Zombie(std::string name);
		~Zombie();

		void announce(void);
};
void randomChump(std::string name);
Zombie* newZombie(std::string name);