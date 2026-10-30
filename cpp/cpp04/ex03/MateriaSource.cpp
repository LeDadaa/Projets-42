/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:52:15 by damalca           #+#    #+#             */
/*   Updated: 2025/11/26 02:59:52 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MateriaSource.hpp"

MateriaSource::MateriaSource() 
{
	for (int i = 0; i < 4; i++)
		_floor[i] = NULL;
}

MateriaSource::~MateriaSource()
{
	for (int i = 0; i < 4; i++)
	{
		if (_floor[i])
			delete (_floor[i]);
	}
}

MateriaSource::MateriaSource(const MateriaSource &a) : IMateriaSource()
{
	for (int i = 0; i < 4; i++)
	{
		if (!a._floor[i])
			_floor[i] = NULL;
		else
			_floor[i] = a._floor[i]->clone();
	}
}

MateriaSource &MateriaSource::operator=(const MateriaSource &b)
{
	for (int i = 0; i < 4; i++)
	{
		if (_floor[i])
			delete(_floor[i]);
		if (b._floor[i])
			_floor[i] = b._floor[i]->clone();
		else
			_floor[i] = NULL;
	}
	return (*this);
}

void MateriaSource::learnMateria(AMateria *materia)
{
	if (!materia)
		return;
	for (int i = 0; i < 4; i++)
	{
		if (!_floor[i])
		{
			_floor[i] = materia;
			std::cout << "MateriaSource learned " << materia->getType() << std::endl;
			return ;
		}
	}
	std::cout << "MateriaSource is full, cannot learn more Materias!" << std::endl;
}

AMateria *MateriaSource::createMateria(std::string const &type)
{
	for (int i = 0; i < 4; i++)
	{
		if (_floor[i] != NULL && _floor[i]->getType() == type)
		{
			std::cout << "MateriaSource created " << type << std::endl;
			return (_floor[i]->clone());
		}
	}
	std::cout << "MateriaSource doesn't know " << type << std::endl;
	return (NULL);
}