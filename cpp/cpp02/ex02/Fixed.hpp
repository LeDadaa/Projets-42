/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 13:19:16 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 12:53:16 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>
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

		bool operator>(const Fixed &nb) const;
		bool operator<(const Fixed &nb) const;
		bool operator>=(const Fixed &nb) const;
		bool operator<=(const Fixed &nb) const;
		bool operator==(const Fixed &nb) const;
		bool operator!=(const Fixed &nb) const;
	
		Fixed operator +(const Fixed &nb) const;
		Fixed operator -(const Fixed &nb) const;
		Fixed operator *(const Fixed &nb) const;
		Fixed operator /(const Fixed &nb) const;

		Fixed &operator++();
		Fixed &operator--();
		Fixed operator++(int);
		Fixed operator--(int);
		
		int getRawBits(void) const;
		void setRawBits(int const raw);
		float toFloat( void ) const;
		int toInt( void ) const;
		
		static Fixed min(Fixed &nb1, Fixed &nb2);
		static const Fixed min(const Fixed &nb1, const Fixed &nb2);
		static Fixed max(Fixed &nb1, Fixed &nb2);
		static const Fixed max(const Fixed &nb1, const Fixed &nb2);
};

std::ostream &operator<<(std::ostream &os, const Fixed &fixed);