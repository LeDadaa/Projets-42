/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:35:28 by damalca           #+#    #+#             */
/*   Updated: 2026/03/16 11:12:35 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serializer.hpp"

int main()
{
	Data original;
	original.id = 42;
	original.name = "damalca";

	Data *ptr = &original;
	uintptr_t raw = Serializer::serialize(ptr);
	Data *restored = Serializer::deserialize(raw);

	std::cout << "original adress: " << &original << std::endl;
	std::cout << "copy adress: " << ptr << std::endl;
	std::cout << "adress in uint: "<< raw << std::endl;
	std::cout << "uint in Data type: " << restored << std::endl;
	std::cout << "restored id: " << restored->id << std::endl;
	std::cout << "restored name: " << restored->name << std::endl;
	return (0);
}