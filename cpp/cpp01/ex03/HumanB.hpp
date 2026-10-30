/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:59:20 by damalca           #+#    #+#             */
/*   Updated: 2025/11/05 10:34:36 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Weapon.hpp"

class HumanB {
	private:
		Weapon* _weapon;
		std::string _name;

	public:
		HumanB(std::string name);
		void setWeapon(Weapon& weapon);
		void attack();
};