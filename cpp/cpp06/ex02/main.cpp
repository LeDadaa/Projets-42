/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:26:49 by damalca           #+#    #+#             */
/*   Updated: 2026/03/19 12:24:11 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base.hpp"

int main()
{	
	std::srand(std::time(NULL));

	Base	*object;

	object = generate();
	std::cout << "identify(Base*): ";
	identify(object);
	std::cout << "identify(Base&): ";
	identify(*object);

	delete object;
	return (0);
}