/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:59:32 by damalca           #+#    #+#             */
/*   Updated: 2025/11/05 09:54:26 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>

class Weapon {
	private:
		std::string _type;

	public:
		Weapon(std::string type);
		const std::string& getType();
		void setType(std::string type);
};