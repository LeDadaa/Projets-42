/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:41:32 by damalca           #+#    #+#             */
/*   Updated: 2025/11/05 11:45:18 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>

class Harl {
	private:
		void debug(void);
		void info(void);
		void warning(void);
		void error(void); 

	public:
		void complain(std::string level);
};