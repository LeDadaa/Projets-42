/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:27:19 by damalca           #+#    #+#             */
/*   Updated: 2026/03/26 15:22:04 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Span.hpp"

Span::Span() : _N(0)
{
}

Span::~Span()
{
}

Span::Span(unsigned int N) : _N(N)
{
}

Span::Span(const Span &src)
{
	*this = src;
}

Span  &Span::operator=(const Span &rdm)
{
	if (this != &rdm)
	{
		this->_N = rdm._N;
		this->_numbers = rdm._numbers;
	}
	return (*this);
}

void Span::addNumber(int number)
{
	if (_numbers.size() >= _N)
		throw SpanFullException();
	_numbers.push_back(number);
}

int Span::shortestSpan() const
{
	if (this->_numbers.size() <= 1)
		throw NoSpanException();

	std::vector<int> sorted(this->_numbers);
	std::sort(sorted.begin(), sorted.end());

	int minSpan = sorted[1] - sorted[0];
	for (std::vector<int>::size_type i = 2; i < sorted.size(); ++i)
	{
		int currSpan = sorted[i] - sorted[i - 1];
		if (currSpan < minSpan)
			minSpan = currSpan;
	}
	return (minSpan);		
}

int Span::longestSpan() const
{
	if (this->_numbers.size() <= 1)
		throw NoSpanException();

	std::vector<int>::const_iterator minIt = std::min_element(this->_numbers.begin(), this->_numbers.end());
	std::vector<int>::const_iterator maxIt = std::max_element(this->_numbers.begin(), this->_numbers.end());

	return (*maxIt - *minIt);
}