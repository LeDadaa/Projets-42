/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:40:19 by damalca           #+#    #+#             */
/*   Updated: 2026/04/01 13:03:24 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: 2 parameters expected." << std::endl;
		return (1);
	}
	try 
	{
		RPN rpn;
		rpn.calculate(av[1]);
	}
    catch (const std::exception& e) 
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}