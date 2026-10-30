/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cure.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:42:49 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 07:20:36 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Amateria.hpp"

class ICharacter;
 
class Cure : public AMateria
{
	public:
		Cure();
		~Cure();
		Cure(const Cure &a);
		Cure &operator=(const Cure &b);
	
		Cure *clone() const;
		void use(ICharacter &target);
};