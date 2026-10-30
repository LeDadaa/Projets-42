/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:37:45 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 14:34:07 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

ClapTrap::ClapTrap() : _name("default"), _hit_points(10), _energy_points(10), _attack_dmg(0)
{
	std::cout << "Default ClapTrap constructor called" << std::endl;
}
ClapTrap::~ClapTrap()
{
	std::cout << "ClapTrap destructor called" << std::endl;
}

ClapTrap::ClapTrap(std::string name) : _name(name), _hit_points(10), _energy_points(10), _attack_dmg(0)
{
	std::cout << "Parametized ClapTrap constructor called" << std::endl;	
}

ClapTrap::ClapTrap(const ClapTrap &a)
{
	std::cout << "ClapTrap copy constructor called" << std::endl;
	*this = a;
}

ClapTrap &ClapTrap::operator=(const ClapTrap &b)
{
	std::cout << "ClapTrap assignment operator called";
	this->_name = b._name;
	this->_hit_points = b._hit_points;
	this->_energy_points = b._energy_points;
	this->_attack_dmg = b._attack_dmg;
	return (*this);
}

void ClapTrap::attack(const std::string& target)
{
	if (this->_energy_points > 0 && this->_hit_points > 0)
	{
		this->_energy_points--;
		std::cout << "ClapTrap " << this->_name << " attacks " << target + "," << " causing "
			<< this->_attack_dmg << " points of damage!" << std::endl;
	}
	else 
		std::cout << "ClapTrap " << this->_name << " has no energy or hit points left" << std::endl;
}

void ClapTrap::takeDamage(unsigned int amount)
{
	if (this->_hit_points > 0)
	{
		if (amount > this->_hit_points)
			this->_hit_points = 0;
		else
			this->_hit_points -= amount;
		std::cout << "ClapTrap " << this->_name << " has received " << amount 
			<< " points of damage and now has " << this->_hit_points << " hit points." << std::endl;
	}
	else
		std::cout << "ClapTrap " << this->_name << " is dead" << std::endl;
}

void ClapTrap::beRepaired(unsigned int amount)
{
	if (this->_energy_points > 0 && this->_hit_points > 0)
	{
		this->_energy_points--;
		this->_hit_points += amount;
		std::cout << "ClapTrap " << this->_name << " has repaired himself for " << amount
			<< " hit points and now has " << this->_hit_points << "." << std::endl;
	}
	else
		std::cout << "ClapTrap " << this->_name << " doesn't have any hit points or energy points left." << std::endl;
}

unsigned int ClapTrap::getHitPoints() const
{
	return (this->_hit_points);
}

unsigned int ClapTrap::getEnergyPoints() const
{
	return (this->_energy_points);
}
		
unsigned int ClapTrap::getAttackDmg() const
{
	return (this->_attack_dmg);
}

std::string ClapTrap::getName() const
{
	return (this->_name);	
}

std::ostream &operator<<(std::ostream &out, const ClapTrap &clap)
{
	out << "Name: " << clap.getName() << std::endl;
	out << "Hit Points: " << clap.getHitPoints() << std::endl;
	out << "Energy Points: " << clap.getEnergyPoints() << std::endl;
	out << "Attack damage: " << clap.getAttackDmg() << std::endl;
	return (out);
}