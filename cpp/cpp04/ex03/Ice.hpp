/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ice.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:42:38 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 07:20:44 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Amateria.hpp"

class ICharacter;

class Ice : public AMateria
{
	public:
		Ice();
		~Ice();
		Ice(const Ice &a);
		Ice &operator=(const Ice &b);
	
		Ice *clone() const;
		void use(ICharacter &target);
};