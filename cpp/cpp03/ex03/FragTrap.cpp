/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:36:43 by damalca           #+#    #+#             */
/*   Updated: 2025/11/21 15:31:44 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fragtrap.hpp"

FragTrap::FragTrap() : ClapTrap("default")
{
	std::cout << "FragTrap default constructor called" << std::endl;
	this->_hit_points = 100;
	this->_energy_points = 100;
	this->_attack_dmg = 30;
}

FragTrap::~FragTrap()
{
	std::cout << "FragTrap destructor called" << std::endl;
}

FragTrap::FragTrap(std::string name) : ClapTrap(name)
{
	std::cout << "FragTrap parametized constructor called" << std::endl;
	this->_hit_points = 100;
	this->_energy_points = 100;
	this->_attack_dmg = 30;
}

FragTrap::FragTrap(const FragTrap &a) : ClapTrap(a)
{
	std::cout << "FragTrap copy constructor called" << std::endl;
	*this = a;
}

FragTrap &FragTrap::operator=(const FragTrap &b)
{
	std::cout << "FragTrap assignment operator called" << std::endl;
	this->_name = b._name;
	this->_hit_points = b._hit_points;
	this->_energy_points = b._energy_points;
	this->_attack_dmg = b._attack_dmg;
	return (*this);
}

void FragTrap::highFivesGuys(void)
{
	std::cout << "High five request accepted!" << std::endl;
}
