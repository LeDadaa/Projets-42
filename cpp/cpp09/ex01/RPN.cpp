/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:30:06 by damalca           #+#    #+#             */
/*   Updated: 2026/04/02 11:39:44 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN()
{
}
		
RPN::RPN(const RPN &src)
{
	*this = src;
}

RPN &RPN::operator=(const RPN &rdm)
{
	if (this != &rdm)
		this->_rpnStack = rdm._rpnStack;
	return (*this);
}
		
RPN::~RPN()
{
}

bool RPN::_isOperator(char c) const
{
	return (c == '+' || c == '-' || c == '/' || c == '*');
}

void RPN::_execOperation(char op)
{
	if (_rpnStack.size() < 2)
		throw (RPN::ErrorException());

	float b = _rpnStack.top();
	_rpnStack.pop();
	float a = _rpnStack.top();
	_rpnStack.pop();

	float result = 0.0f;

	if (op == '+')
		result = a + b;
	else if (op == '-')
		result = a - b;
	else if (op == '*')
		result = a * b;
	else if (op == '/')
	{
		if (b == 0)
			throw (RPN::ErrorException());
		result = a / b;
	}
	_rpnStack.push(result);
}

void RPN::calculate(const std::string &entry)
{
	std::stringstream ss(entry);
	std::string 	  token;

	while (ss >> token)
	{
		if (token.length() != 1)
			throw (RPN::ErrorException());
		
		char c = token[0];

		if (c >= '0' && c <= '9')
			_rpnStack.push(c - '0');
		else if (_isOperator(c))
			_execOperation(c);
		else
			throw (RPN::ErrorException());
	}
	if (_rpnStack.size() != 1)
		throw (RPN::ErrorException());

	std::cout << _rpnStack.top() << std::endl;
}