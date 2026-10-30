/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 11:50:47 by damalca           #+#    #+#             */
/*   Updated: 2026/03/24 13:57:57 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <exception>

template <typename T>
class Array 
{
	private:
		T 			 *_array;
		unsigned int _size;
	
	public:
		Array() : _size(0)
		{
			this->_array = new T[0];
		}
		
		~Array()
		{
			if (this->_array)
				delete [] this->_array;
		}
		
		Array(unsigned int size) : _size(size)
		{
			this->_array = new T[_size];
		}
		
		Array(const Array &src) : _size(src._size)
		{
			this->_array = NULL;
			*this = src;	
		}
		
		Array &operator=(const Array &src)
		{
			if (this == &src)
				return (*this);
			if (this->_array)
					delete [] this->_array;
			this->_size = src._size;
			this->_array = new T[this->_size];
			for (unsigned int i = 0; i < this->_size; i++)
				this->_array[i] = src._array[i];
			return (*this);
		}
		
		T &operator[](unsigned int index)
		{
			if (index >= this->_size)
				throw InvalidIndexException();
			return (this->_array[index]);
		}

		const T &operator[](unsigned int index) const
		{
			if (index >= this->_size)
				throw InvalidIndexException();
			return (this->_array[index]);
		}

		class InvalidIndexException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Array: index out of range");
				}
		};

		unsigned int size() const
		{
			return (this->_size);
		}
};



