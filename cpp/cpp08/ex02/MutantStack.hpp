/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MutantStack.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 11:31:24 by damalca           #+#    #+#             */
/*   Updated: 2026/03/27 11:45:42 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stack>

template <typename T>
class MutantStack : public std::stack<T>
{
	public:
		typedef typename std::stack<T>::container_type::iterator iterator;
		typedef typename std::stack<T>::container_type::const_iterator const_iterator;
		typedef typename std::stack<T>::container_type::reverse_iterator reverse_iterator;
		typedef typename std::stack<T>::container_type::const_reverse_iterator const_reverse_iterator;

		MutantStack() : std::stack<T>() 
		{
		}
		~MutantStack() 
		{
		}
		MutantStack(const MutantStack &src) : std::stack<T>(src) 
		{			
		}
		MutantStack &operator=(const MutantStack &rdm)
		{
			std::stack<T>::operator=(rdm);
			return (*this);
		}

		iterator begin() 
		{ 
			return this->c.begin(); 
		}
		iterator end()
		{ 
			return this->c.end(); 
		}
		const_iterator begin() const 
		{ 
			return this->c.begin(); 
		}
		const_iterator end() const 
		{ 
			return this->c.end();
		}
		reverse_iterator rbegin() 
		{ 
			return this->c.rbegin(); 
		}
		reverse_iterator rend() 
		{ 
			return this->c.rend();
		}
		const_reverse_iterator rbegin() const 
		{ 
			return this->c.rbegin(); 
		}
		const_reverse_iterator rend() const 
		{ 
			return this->c.rend(); 
		}
};