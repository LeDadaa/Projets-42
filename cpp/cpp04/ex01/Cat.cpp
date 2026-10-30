/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:52:15 by damalca           #+#    #+#             */
/*   Updated: 2025/11/24 13:02:31 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

Cat::Cat() : Animal("Cat")
{
	std::cout << "Cat default constructor called" << std::endl;
	this->_brain = new Brain();
}

Cat::~Cat()
{
	std::cout << "Cat destructor called" << std::endl;
	delete (this->_brain);
}
Cat::Cat(std::string name) : Animal(name)
{
	std::cout << "Cat parametized constructor called" << std::endl;
	this->_brain = new Brain();
}

Cat::Cat(const Cat &a) : Animal(a)
{
	std::cout << "Cat copy constructor called" << std::endl;
	this->_brain = new Brain(*a._brain);
}

Cat &Cat::operator=(const Cat &b)
{
	if (this != &b)
	{
		this->_type = b._type;
		delete (this->_brain);
		this->_brain = new Brain(*b._brain);
	}
	return (*this);
}

void Cat::makeSound() const
{
	std::cout << "Meow meow!" << std::endl;
}

Brain *Cat::getBrain() const
{
	return (this->_brain);
}
