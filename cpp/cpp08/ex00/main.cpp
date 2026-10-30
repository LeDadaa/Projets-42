/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 10:06:01 by damalca           #+#    #+#             */
/*   Updated: 2026/03/27 12:09:17 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easyfind.hpp"
#include <vector>
#include <iostream>

int main()
{
	std::cout << "=== MY TESTS ===" << std::endl;

	std::vector<int> tab;
	tab.push_back(10);
	tab.push_back(458);
	tab.push_back(-5);
	tab.push_back(42);

	try
	{
		std::vector<int>::iterator iter = easyfind(tab, 42);
		std::cout << "Found: " << *iter << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		std::vector<int>::iterator iter = easyfind(tab, -5);
		std::cout << "Found: " << *iter << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	try
	{
		std::vector<int>::iterator iter = easyfind(tab, 41);
		std::cout << "Found: " << *iter << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}