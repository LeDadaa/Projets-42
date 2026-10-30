/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:57:28 by damalca           #+#    #+#             */
/*   Updated: 2025/11/05 10:41:22 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanB.hpp"

HumanB::HumanB (std::string name) :  _weapon(NULL), _name(name){}

void HumanB::setWeapon(Weapon &weapon) { _weapon = &weapon; }

void HumanB::attack() 
{
	if (_weapon) { std::cout << _name << " attacks with " << _weapon->getType() << std::endl; }
	
	else { std ::cout << _name << " attacks with his own hands " << std::endl; }
}
