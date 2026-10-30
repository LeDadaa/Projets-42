/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:27:11 by damalca           #+#    #+#             */
/*   Updated: 2026/03/25 14:39:48 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <exception>
#include <vector>

class Span
{
	private:
		unsigned int		_N;
		std::vector<int>	_numbers;
	
	public:
		Span();
		~Span();
		Span(unsigned int N);
		Span(const Span &src);
		Span &operator=(const Span &rdm);
	
		void addNumber(int number);
		int shortestSpan() const;
		int longestSpan() const;

		template <typename InputIterator>
		void addRange(InputIterator begin, InputIterator end)
		{
			for (InputIterator it = begin; it != end; ++it)
				addNumber(*it);
		}

		class SpanFullException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Span is full");
				}
		};

		class NoSpanException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Less than 2 numbers in Span");
				}
		};
};