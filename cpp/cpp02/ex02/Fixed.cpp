/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 13:19:22 by damalca           #+#    #+#             */
/*   Updated: 2025/11/10 19:02:10 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed() : _number(0) {}

Fixed::~Fixed() {}

Fixed::Fixed(const Fixed &random) : _number(random._number) {}

Fixed &Fixed::operator=(const Fixed &src)
{
	this->_number = src._number;
	return (*this);
}

bool Fixed::operator>(const Fixed &nb) const { return this->_number > nb._number; }

bool Fixed::operator<(const Fixed &nb) const { return this->_number < nb._number; }

bool Fixed::operator>=(const Fixed &nb) const { return this->_number >= nb._number; }

bool Fixed::operator<=(const Fixed &nb) const { return this->_number <= nb._number; }

bool Fixed::operator==(const Fixed &nb) const { return this->_number == nb._number; }

bool Fixed::operator!=(const Fixed &nb) const { return this->_number != nb._number; }
	
Fixed Fixed::operator+(const Fixed &nb) const {
	Fixed result;
	result.setRawBits(this->_number + nb._number);
	return result;
}

Fixed Fixed::operator-(const Fixed &nb) const {
	Fixed result;
	result.setRawBits(this->_number - nb._number);
	return result;
}

Fixed Fixed::operator*(const Fixed &nb) const {
	Fixed result;
	result.setRawBits((this->_number * nb._number) >> _bits);
	return result;
}

Fixed Fixed::operator/(const Fixed &nb) const {
	Fixed result;
	result.setRawBits((this->_number << _bits) / nb._number);
	return result;
}

Fixed &Fixed::operator++() { this->_number++; return (*this); }

Fixed &Fixed::operator--() { this->_number--; return (*this); }

Fixed Fixed::operator++(int) { Fixed tmp (*this); this->_number++; return (tmp); }

Fixed Fixed::operator--(int) { Fixed tmp (*this); this->_number--; return (tmp); }

int Fixed::getRawBits(void) const { return (_number); }

void Fixed::setRawBits(int const raw) {_number = raw;}

Fixed::Fixed (const int nb) { setRawBits(nb << _bits); }

Fixed::Fixed (const float nb){ setRawBits(roundf(nb * (1 << _bits))); }

float Fixed::toFloat( void ) const { return ((float)this->_number / (1 << _bits)); }

int Fixed::toInt( void ) const { return (this->_number / (1 << _bits)); }

Fixed Fixed::min(Fixed &nb1, Fixed &nb2) { if (nb1 < nb2) return (nb1); else return (nb2); }

const Fixed Fixed::min(const Fixed &nb1, const Fixed &nb2) { if (nb1 < nb2) return (nb1); else return (nb2); }

Fixed Fixed::max(Fixed &nb1, Fixed &nb2) { if (nb1 < nb2) return (nb2); else return (nb1); }

const Fixed Fixed::max(const Fixed &nb1, const Fixed &nb2) { if (nb1 < nb2) return (nb2); else return (nb1); }

std::ostream &operator<<(std::ostream &os, const Fixed &fixed) { return (os << fixed.toFloat()); }