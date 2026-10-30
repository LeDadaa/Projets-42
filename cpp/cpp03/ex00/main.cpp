/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:37:26 by damalca           #+#    #+#             */
/*   Updated: 2025/11/19 15:26:46 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

int	main(void)
{
	ClapTrap clap("ledada");

	std::cout << clap << std::endl;
	clap.attack("pigeon");
	clap.beRepaired(1);
	clap.takeDamage(5);
	clap.beRepaired(1);
	std::cout << std::endl;
	std::cout << clap << std::endl;
	ClapTrap clap2(clap);
	std::cout << clap2 << std::endl;
}