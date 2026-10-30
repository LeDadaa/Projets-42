/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:15:59 by damalca           #+#    #+#             */
/*   Updated: 2025/11/05 09:48:42 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

int main()
{
	Zombie *zombie;
	zombie = zombieHorde(5, "Pierre");
	for (int i = 0; i < 5; i++)
	{
		std::cout << "voici le nom du zombie numero " << i + 1 << ": " <<  zombie->getName() << std::endl;
	}
	delete[] zombie;
}
