/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 13:19:16 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 12:45:21 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cmath>

class Fixed {
	private: 
		int _number;
		static const int _bits = 8;
	public:
		Fixed();
		Fixed(const Fixed &random);
		Fixed &operator=(const Fixed &src);
		~Fixed();
		Fixed (const int nb);
		Fixed (const float nb);

		int getRawBits(void) const;
		void setRawBits(int const raw);
		float toFloat( void ) const;
		int toInt( void ) const;
};

std::ostream &operator<<(std::ostream &os, const Fixed &fixed);