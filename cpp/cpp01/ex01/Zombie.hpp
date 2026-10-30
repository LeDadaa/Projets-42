/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:20:08 by damalca           #+#    #+#             */
/*   Updated: 2025/11/26 04:33:59 by damalca          ###   ########.fr       */
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
		Zombie();
		Zombie(std::string name);
		~Zombie();
		void announce(void);
		std::string getName();
		void setName(std::string name);
};
Zombie* zombieHorde( int N, std::string name );
