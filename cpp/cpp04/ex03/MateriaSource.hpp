/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:52:06 by damalca           #+#    #+#             */
/*   Updated: 2025/11/26 03:00:07 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Amateria.hpp"
#include "Character.hpp"
#include "IMateriaSource.hpp"

class IMateriaSource;

class MateriaSource : public IMateriaSource
{
	private:
		AMateria *_floor[4];
		
	public:
		MateriaSource();
		~MateriaSource();
		MateriaSource(const MateriaSource &a);
		MateriaSource &operator=(const MateriaSource &b);

		void learnMateria(AMateria *materia);
		AMateria *createMateria(std::string const &type);
};