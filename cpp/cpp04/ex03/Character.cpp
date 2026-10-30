/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:51:04 by damalca           #+#    #+#             */
/*   Updated: 2026/02/22 13:58:38 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Character.hpp"

Character::Character() : _name("Default name"), _floorCount(0)
{
	for (int i = 0; i < 4; i++)
		this->_inventory[i] = NULL;
	for (int i = 0; i < 100; i++)
		this->_floor[i] = NULL;
}

Character::~Character()
{
	for (int i = 0; i < 4; i++)
	{
		if (this->_inventory[i])
			delete(this->_inventory[i]);
	}
	for (int i = 0; i < _floorCount; i++)
	{
		if (this->_floor[i])
			delete(this->_floor[i]);
	}
}

Character::Character(const std::string &name) : _name(name), _floorCount(0)
{
	for (int i = 0; i < 4; i++)
		this->_inventory[i] = NULL;
	for (int i = 0; i < 100; i++)
		this->_floor[i] = NULL;
}
		
Character::Character(const Character &a) : _name(a._name), _floorCount(0)
{
	for (int i = 0; i < 4; i++)
	{
		if (!a._inventory[i])
			_inventory[i] = NULL;
		else
			_inventory[i] = a._inventory[i]->clone();
	}
	for (int i = 0; i < 100; i++)
		this->_floor[i] = NULL;
}
		
Character &Character::operator=(const Character &b)
{
	if (this != &b)
	{
		_name = b._name;
		for (int i = 0; i < 4; i++)
		{
			if (this->_inventory[i])
				delete (this->_inventory[i]);
			if (b._inventory[i])
				this->_inventory[i] = (b._inventory[i])->clone();
			else
				this->_inventory[i] = NULL;
		}
	}
	return (*this);
}

const std::string &Character::getName() const
{
	return (_name);	
}

AMateria *Character::getMateria(int idx) const
{
	if (idx < 0 || idx > 3)
	{
		std::cout << _name << ": invalid index." << std::endl;
		return (NULL);
	}
	if (_inventory[idx])
		return (_inventory[idx]);
	else
	{
		std::cout << _name << ": no Materia found at index " << idx << "." << std::endl;
		return (NULL);
	}
}

void Character::equip(AMateria *m)
{
	if (!m)
	{
		std::cout << _name << ": can't equip a non-existent Materia." << std::endl;
		return ;
	}
	for (int i = 0; i < 4; i++)
	{
		if (!_inventory[i])
		{
			_inventory[i] = m;
			std::cout << _name << " has equipped " << m->getType() + "." << std::endl;
			return ;
		}
	}
	std::cout << _name << " has no place left in inventory." << std::endl;
}

void Character::unequip(int idx)
{
	if (idx < 0 || idx > 3)
	{
		std::cout << _name << ": invalid index." << std::endl;
		return ;
	}
	if (_inventory[idx])
	{
		std::cout << _name << " has unequipped " << _inventory[idx]->getType() + "." << std::endl;
		if (_floorCount < 100)
		{
			_floor[_floorCount] = _inventory[idx];
			_floorCount++;
		}
		_inventory[idx] = NULL;
		return ;
	}
	std::cout << _name << " has no Materia at index " << idx << "." << std::endl;
}

void Character::use(int idx, ICharacter &target)
{
	if (_inventory[idx])
	{
		_inventory[idx]->use(target);
		return ;
	}
	std::cout << _name << " has no Materia at index " << idx << "." << std::endl;
}