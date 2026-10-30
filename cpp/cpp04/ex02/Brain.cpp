/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:16:38 by damalca           #+#    #+#             */
/*   Updated: 2025/11/24 12:42:10 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Brain.hpp"

Brain::Brain()
{
	std::cout << "Brain default constructor called" << std::endl;
	for (int i = 0; i < 100; i++)
		_ideas[i] = "default";
}

Brain::~Brain()
{
	std::cout << "Brain destructor called" << std::endl;
}

Brain::Brain(const Brain &a)
{
	std::cout << "Brain copy constructor called" << std::endl;
	for (int i = 0; i < 100; i++)
		this->_ideas[i] = a._ideas[i];
}

Brain &Brain::operator=(const Brain &b)
{
	for (int i = 0; i < 100; i++)
		this->_ideas[i] = b._ideas[i];
	return (*this);
}

void Brain::initIdeas(const std::string idea)
{
	for (int i = 0; i < 100; i++)
		this->_ideas[i] = idea;
}

void Brain::printIdeas() const
{
	for (int i = 0; i < 100; i++)
		std::cout << this->_ideas[i] << std::endl;
}