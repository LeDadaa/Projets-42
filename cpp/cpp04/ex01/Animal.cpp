/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:49:26 by damalca           #+#    #+#             */
/*   Updated: 2025/11/24 12:02:17 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

Animal::Animal() : _type("default")
{
	std::cout << "Animal default constructor called" << std::endl;
}

Animal::~Animal()
{
	std::cout << "Animal destructor called" << std::endl;
}
Animal::Animal(std::string type) : _type(type)
{
		std::cout << "Animal parametized constructor called" << std::endl;
}

Animal::Animal(const Animal &a)
{
	std::cout << "Animal copy constructor called" << std::endl;
	*this = a;
}

Animal &Animal::operator=(const Animal &b)
{
	this->_type = b._type;
	return (*this);
}

void Animal::makeSound() const
{
	std::cout << "Random animal sound." << std::endl;
}

std::string Animal::getType() const
{
	return (this->_type);
}