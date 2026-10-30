/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:52:15 by damalca           #+#    #+#             */
/*   Updated: 2026/02/22 13:06:14 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

Cat::Cat() : Animal("Cat")
{
	std::cout << "Cat default constructor called" << std::endl;
}

Cat::~Cat()
{
	std::cout << "Cat destructor called" << std::endl;
}
Cat::Cat(std::string name) : Animal(name)
{
	std::cout << "Cat parametized constructor called" << std::endl;
}

Cat::Cat(const Cat &a) : Animal(a)
{
	std::cout << "Cat copy constructor called" << std::endl;
}

Cat &Cat::operator=(const Cat &b)
{
	std::cout << "Cat assignment operator called" << std::endl;
	if (this != &b)
		this->_type = b._type;
	return (*this);
}

void Cat::makeSound() const
{
	std::cout << "Meow meow!" << std::endl;
}
