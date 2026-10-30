/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cure.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:42:54 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 22:22:53 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cure.hpp"

Cure::Cure() : AMateria("cure") {}

Cure::~Cure() {}

Cure::Cure(const Cure &a) : AMateria(a) {}

Cure &Cure::operator=(const Cure &b)
{
	if (this != &b)
		_type = b._type;
	return (*this);
}
	
Cure *Cure::clone() const
{
	return (new Cure());
}
		
void Cure::use(ICharacter &target)
{
	std::cout << "* heals " << target.getName() + " wounds *" << std::endl;
}