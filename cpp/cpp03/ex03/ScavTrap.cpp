/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 11:36:33 by damalca           #+#    #+#             */
/*   Updated: 2025/11/19 15:24:19 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"

ScavTrap::ScavTrap() : ClapTrap("default"), _guardGateMode(false)
{
	std::cout << "ScavTrap default constructor called" << std::endl;
	this->_hit_points = 100;
	this->_energy_points = 50;
	this->_attack_dmg = 20;
}

ScavTrap::~ScavTrap()
{
	std::cout << "ScavTrap destructor called" << std::endl;
}

ScavTrap::ScavTrap(std::string name) : ClapTrap(name), _guardGateMode(false)
{
	std::cout << "ScavTrap parametized constructor called" << std::endl;
	this->_hit_points = 100;
	this->_energy_points = 50;
	this->_attack_dmg = 20;
}

ScavTrap::ScavTrap(const ScavTrap &a) : ClapTrap(a), _guardGateMode(false)
{
	std::cout << "ScavTrap copy constructor called" << std::endl;
	*this = a;
}

ScavTrap &ScavTrap::operator=(const ScavTrap &b)
{
	std::cout << "ScavTrap assignment operator called" << std::endl;
	this->_name = b._name;
	this->_hit_points = b._hit_points;
	this->_energy_points = b._energy_points;
	this->_attack_dmg = b._attack_dmg;
	this->_guardGateMode = b._guardGateMode;
	return (*this);
}

void ScavTrap::attack(const std::string &target)
{
	if (this->_energy_points == 0 || this->_hit_points == 0)
	{
		std::cout << "ScavTrap doesn't have any energy or hit points left" << std::endl;
		return ;
	}
	else
	{
		this->_energy_points--;
		std::cout << "ScavTrap " << this->_name << " attacks " << target + ","
			<< " causing " << this->_attack_dmg << " points of damage!" << std::endl;
	}
}

void ScavTrap::guardGate(void)
{
	if (!this->_guardGateMode)
	{
		std::cout << "ScavTrap " << this->_name << " is now in gate keeper mode." 
			<< std::endl;
		this->_guardGateMode = true;
		return ;
	}
	std::cout << "ScavTrap " << this->_name << " is already in gate keeper mode." 
		<< std::endl;
}