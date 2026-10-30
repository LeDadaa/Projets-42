/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ice.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:42:32 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 22:22:39 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ice.hpp"

Ice::Ice() : AMateria("ice") {}

Ice::~Ice() {}

Ice::Ice(const Ice &a) : AMateria(a) {}

Ice &Ice::operator=(const Ice &b)
{
	if (this != &b)
		_type = b._type;
	return (*this);
}
	
Ice *Ice::clone() const
{
	return (new Ice());
}
		
void Ice::use(ICharacter &target)
{
	std::cout << "* shoots an ice bolt at " << target.getName() + " *" << std::endl;
}