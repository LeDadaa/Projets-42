/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:03:28 by damalca           #+#    #+#             */
/*   Updated: 2025/11/24 12:11:25 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongCat.hpp"

WrongCat::WrongCat() : WrongAnimal("WrongCat")
{
	std::cout << "WrongCat default constructor called" << std::endl;
}

WrongCat::~WrongCat()
{
	std::cout << "WrongCat destructor called" << std::endl;
}
WrongCat::WrongCat(std::string name) : WrongAnimal(name)
{
	std::cout << "WrongCat parametized constructor called" << std::endl;
}

WrongCat::WrongCat(const WrongCat &a)
{
	std::cout << "WrongCat copy constructor called" << std::endl;
	*this = a;
}

WrongCat &WrongCat::operator=(const WrongCat &b)
{
	this->_type = b._type;
	return (*this);
}

void WrongCat::makeSound() const
{
	std::cout << "Wrong meow meow!" << std::endl;
}