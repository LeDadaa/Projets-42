/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:52:06 by damalca           #+#    #+#             */
/*   Updated: 2025/11/24 12:49:37 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include "Animal.hpp"
#include "Brain.hpp"

class Cat : public Animal
{
	private:
		Brain *_brain;
	
	public:
		Cat();
		~Cat();
		Cat(std::string name);
		Cat(const Cat &a);
		Cat &operator=(const Cat &b);
	
	void makeSound() const;
	Brain* getBrain() const;
};