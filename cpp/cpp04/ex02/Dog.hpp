/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:51:49 by damalca           #+#    #+#             */
/*   Updated: 2025/11/24 12:49:52 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include "Animal.hpp"
#include "Brain.hpp"

class Dog : public Animal
{
	private:
		Brain *_brain;

	public:
		Dog();
		~Dog();
		Dog(std::string name);
		Dog(const Dog &a);
		Dog &operator=(const Dog &b);
	
	void makeSound() const;
	Brain* getBrain() const;
};
