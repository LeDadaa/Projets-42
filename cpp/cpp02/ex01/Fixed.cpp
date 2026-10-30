/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 13:19:22 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 12:45:42 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed() : _number(0) 
{
	std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const Fixed &random) : _number(random._number)
{
	std::cout << "Copy constructor called" << std::endl;
}

Fixed &Fixed::operator=(const Fixed &src)
{
	std::cout << "Copy assignement operator called" << std::endl;
	this->_number = src._number;
	return (*this);
}

Fixed::~Fixed()
{
	std::cout << "Destructor called" << std::endl;
	
}

int Fixed::getRawBits(void) const
{
	std::cout << "getRawBits member function called" << std::endl;
	return (_number);
}

void Fixed::setRawBits(int const raw)
{
	_number = raw;
}

Fixed::Fixed (const int nb)
{
	std::cout << "Int constructor called" << std::endl;
	setRawBits(nb << _bits);
}

Fixed::Fixed (const float nb)
{
	std::cout << "Float constructor called" << std::endl;
	setRawBits(roundf(nb * (1 << _bits)));
}

float Fixed::toFloat( void ) const
{
	return ((float)this->_number / (1 << _bits));
}

int Fixed::toInt( void ) const
{
	return (this->_number / (1 << _bits));
}

std::ostream &operator<<(std::ostream &os, const Fixed &fixed)
{
	return (os << fixed.toFloat());
}