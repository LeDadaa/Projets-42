/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 11:23:23 by damalca           #+#    #+#             */
/*   Updated: 2026/03/20 16:17:19 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iter.hpp"

template <typename T>
void printTab(const T &value)
{
	std::cout << value << " ";
}

void incrementInt(int &value)
{
	++value;
}

void addInterrogation(std::string &value)
{
	value += "?";
}

void multiplyDouble(double &value)
{
	value *= 2.0;
}

int main()
{
	int numbers[] = {1, 2, 3, 4};
	std::string words[] = {"hello", ".", "cpp", "42"};
	double doubles[] = {1.5, 2.5, 3.5};

	std::cout << "Original int array: ";
	iter(numbers, 4, printTab<int>);
	std::cout << std::endl;

	iter(numbers, 4, incrementInt);
	std::cout << "Int array after increment (non-const ref): ";
	iter(numbers, 4, printTab<int>);
	std::cout << std::endl;

	std::cout << "Original string array: ";
	iter(words, 4, printTab<std::string>);
	std::cout << std::endl;

	iter(words, 4, addInterrogation);
	std::cout << "String array after modifs (non-const ref): ";
	iter(words, 4, printTab<std::string>);
	std::cout << std::endl;

	std::cout << "Double array before muliplication: ";
	iter(doubles, 3, printTab<double>);
	std::cout << std::endl;

	iter(doubles, 3, multiplyDouble);
	std::cout << "Mutable double array multiplication: ";
	iter(doubles, 3, printTab<double>);
	std::cout << std::endl;
}