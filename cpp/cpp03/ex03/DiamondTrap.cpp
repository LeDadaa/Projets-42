/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiamondTrap.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:02:55 by damalca           #+#    #+#             */
/*   Updated: 2025/11/21 16:44:15 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap() : ClapTrap("default_clap_name"), _name("default")
{
	std::cout << "Default DiamondTrap constructor called" << std::endl;
	this->_hit_points = FragTrap::_hit_points;
	this->_energy_points = ScavTrap::_energy_points;
	this->_attack_dmg = FragTrap::_attack_dmg;
}

DiamondTrap::~DiamondTrap()
{
	std::cout << "DiamondTrap destructor called" << std::endl;
}

DiamondTrap::DiamondTrap(std::string name): ClapTrap(name + "_clap_name"), _name(name)
{
	std::cout << "DiamondTrap parametized constructor called" << std::endl;
	this->_hit_points = FragTrap::_hit_points;
	this->_energy_points = ScavTrap::_energy_points;
	this->_attack_dmg = FragTrap::_attack_dmg;
}

DiamondTrap::DiamondTrap(const DiamondTrap &a) : ClapTrap(a), FragTrap(a), ScavTrap(a)
{
	std::cout << "DiamondTrap copy constructor called" << std::endl;
	*this = a;
}

DiamondTrap &DiamondTrap::operator=(const DiamondTrap &b)
{
	std::cout << "DiamondTrap assignment operator called" << std::endl;
	this->_name = b._name;
	ClapTrap::_name = b.ClapTrap::_name;
	this->_hit_points = b._hit_points;
	this->_energy_points = b._energy_points;
	this->_attack_dmg = b._attack_dmg;
	return (*this);	
}
	
void DiamondTrap::attack(const std::string &target)
{
	ScavTrap::attack(target);
}

void DiamondTrap::WhoAmI()
{
	std::cout << "DiamondTrap name: " << this->_name
		<< " and its ClapTrap name: " << ClapTrap::_name << std::endl;
}
