/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:37:26 by damalca           #+#    #+#             */
/*   Updated: 2025/11/19 15:25:07 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"

int	main(void)
{
	ScavTrap trap("yes");

	std::cout << trap << std::endl;
	trap.attack("pigeon");
	trap.takeDamage(36);
	trap.beRepaired(18);
	trap.takeDamage(20);
	trap.beRepaired(10);
	trap.guardGate();
	trap.guardGate();
	std::cout << std::endl;
	std::cout << trap << std::endl;
	ScavTrap trap2(trap);
	std::cout << trap2 << std::endl;
}