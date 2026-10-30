/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:51:58 by damalca           #+#    #+#             */
/*   Updated: 2025/11/24 13:07:33 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"

Dog::Dog() : Animal("Dog")
{
	std::cout << "Dog default constructor called" << std::endl;
	this ->_brain = new Brain();
}

Dog::~Dog()
{
	std::cout << "Dog destructor called" << std::endl;
	delete (this->_brain);
}
Dog::Dog(std::string name) : Animal(name)
{
	std::cout << "Dog parametized constructor called" << std::endl;
	this->_brain = new Brain();
}

Dog::Dog(const Dog &a) : Animal(a)
{
	std::cout << "Dog copy constructor called" << std::endl;
	this->_brain = new Brain(*a._brain);
}

Dog &Dog::operator=(const Dog &b)
{
	if (this != &b)
	{
		this->_type = b._type;
		delete (this->_brain);
		this->_brain = new Brain(*b._brain);
	}
	return (*this);
}

void Dog::makeSound() const
{
	std::cout << "Woof Woof!" << std::endl;
}

Brain *Dog::getBrain() const
{
	return (this->_brain);
}