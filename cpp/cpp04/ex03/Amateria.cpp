/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Amateria.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:40:56 by damalca           #+#    #+#             */
/*   Updated: 2025/11/26 02:51:45 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Amateria.hpp"

AMateria::AMateria() {}

AMateria::~AMateria() {}

AMateria::AMateria(const std::string &type) : _type(type) {}

AMateria::AMateria(const AMateria &a) : _type(a._type) {}

std::string const &AMateria::getType() const
{
	return (this->_type);
}
		
void AMateria::use(ICharacter& target)
{
	std::cout << "Default attacks on " << target.getName() << std::endl;
}