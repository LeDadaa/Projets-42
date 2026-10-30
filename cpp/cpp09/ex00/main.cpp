/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 12:47:43 by damalca           #+#    #+#             */
/*   Updated: 2026/03/31 09:59:21 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}
	try 
	{
		BitcoinExchange btc("data.csv");
		btc.processInputFile(av[1]);
	}
    catch (const std::exception& e) 
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}