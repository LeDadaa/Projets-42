/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:28:24 by damalca           #+#    #+#             */
/*   Updated: 2026/03/27 12:07:15 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Span.hpp"
#include <iostream>
#include <vector>

int main()
{
	std::cout << "=== MAIN TESTS ===" << std::endl;
	try
	{
		Span sp(5);
		sp.addNumber(6);
		sp.addNumber(3);
		sp.addNumber(17);
		sp.addNumber(9);
		sp.addNumber(11);
		std::cout << "Shortest: " << sp.shortestSpan() << std::endl;
		std::cout << "Longest: " << sp.longestSpan() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n=== TEST EXCEPTION SPAN TOO LOW ===" << std::endl;
	try
	{
		Span one(3);
		one.addNumber(42);
		std::cout << one.shortestSpan() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl; 
	}

	std::cout << "\n=== TEST EXCEPTION SPAN FULL ===" << std::endl;
	try
	{
		Span full(2);
		full.addNumber(1);
		full.addNumber(2);
		full.addNumber(3);
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n=== TEST ADDRANGE ===" << std::endl;
	try
	{
		std::vector<int> values;
		values.push_back(100);
		values.push_back(105);
		values.push_back(150);
		values.push_back(151);
		values.push_back(200);
		Span rangeSpan(values.size());
		rangeSpan.addRange(values.begin(), values.end());
		std::cout << "Shortest: " << rangeSpan.shortestSpan() << std::endl;
		std::cout << "Longest : " << rangeSpan.longestSpan() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Erreur: " << e.what() << std::endl;
	}

	std::cout << "\n=== TEST BIG TAB ===" << std::endl;
	try
	{
		const int size = 20000;
		Span big(size);
		for (int i = 0; i < size; ++i)
			big.addNumber(i * 3);
		std::cout << "Elements: " << size << std::endl;
		std::cout << "Shortest: " << big.shortestSpan() << std::endl;
		std::cout << "Longest : " << big.longestSpan() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Erreur: " << e.what() << std::endl;
	}
}