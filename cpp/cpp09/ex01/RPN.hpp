/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:30:13 by damalca           #+#    #+#             */
/*   Updated: 2026/04/02 11:39:15 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stack>
#include <string>
#include <iostream>
#include <exception>
#include <sstream>

class RPN
{
	private:
		std::stack<float> _rpnStack;
		
		bool _isOperator(char c) const;
		void _execOperation(char op);

	public:
		RPN();
		RPN(const RPN &src);
		RPN& operator=(const RPN &rdm);
		~RPN();

		void calculate(const std::string &entry);

	class ErrorException : public std::exception 
	{
    public:
        virtual const char* what() const throw() 
		{
            return ("Error");
        }
	};
};