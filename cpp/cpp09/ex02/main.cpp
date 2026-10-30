/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 10:43:57 by damalca           #+#    #+#             */
/*   Updated: 2026/04/02 11:35:43 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int ac, char **av)
{
	if (ac <= 2)
	{
		std::cerr << "Error: At least 2 numbers expected." << std::endl;
		return (1);
	}
	try
	{
		PmergeMe args;
		args.execute(ac - 1, av + 1);
	}
	catch (const std::exception& e) 
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);

}