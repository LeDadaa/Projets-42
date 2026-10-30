/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 13:19:05 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 13:00:18 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

int main(void) 
{	Fixed a;
	Fixed const b(Fixed(5.05f) * Fixed(2));
	
	std::cout << a << std::endl;
	std::cout << ++a << std::endl;
	std::cout << a << std::endl;
	std::cout << a++ << std::endl;
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << Fixed::max(a, b) << std::endl;
	
	std::cout << "\n=== TESTS DES OPERATEURS DE COMPARAISON ===" << std::endl;
	Fixed x(10);
	Fixed y(5);
	Fixed z(10);
	
	std::cout << x << y << z << std::endl;
	std::cout << (x > y ? "true" : "false") << std::endl;
	std::cout << (x < y ? "true" : "false") << std::endl;
	std::cout << (x >= z ? "true" : "false") << std::endl;
	std::cout << (x <= z ? "true" : "false") << std::endl;
	std::cout << (x == z ? "true" : "false") << std::endl;
	std::cout << (x != y ? "true" : "false") << std::endl;
	
	Fixed n1(10);
	Fixed n2(3);
	std::cout << n2 << std::endl;
	std::cout << (n1 + n2) << std::endl;
	std::cout << (n1 - n2) << std::endl;
	std::cout << (n1 * n2) << std::endl;
	std::cout << (n1 / n2) << std::endl;

	Fixed c(5);
	std::cout << c << std::endl;
	std::cout << ++c << std::endl;
	std::cout <<  c << std::endl;
	std::cout <<  c++ << std::endl;
	std::cout <<  c << std::endl;
	std::cout << --c << std::endl;
	std::cout <<  c << std::endl;
	std::cout <<  c-- << std::endl;
	std::cout << c << std::endl;

	Fixed m1(42.42f);
	Fixed m2(10.5f);
	std::cout << m1 << m2 << std::endl;
	std::cout << Fixed::min(m1, m2) << std::endl;
	std::cout << Fixed::max(m1, m2) << std::endl;
	
	Fixed const m3(100.5f);
	Fixed const m4(200.25f);
	std::cout << m3 <<  m4 << std::endl;
	std::cout << Fixed::min(m3, m4) << std::endl;
	std::cout << Fixed::max(m3, m4) << std::endl;
	
	return (0);
}