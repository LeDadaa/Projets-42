/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:19:12 by damalca           #+#    #+#             */
/*   Updated: 2026/03/11 12:19:12 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter() {}

ScalarConverter::ScalarConverter(const ScalarConverter &src)
{
	(void)src;
}

ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other)
{
	(void)other;
	return (*this);
}

ScalarConverter::~ScalarConverter() {}

static bool isPseudoLiteral(const std::string &s)
{
	return (s == "nan" || s == "nanf" || s == "+inf"
			|| s == "-inf" || s == "+inff" || s == "-inff");
}

static int getType(const std::string &s)
{
	if (isPseudoLiteral(s))
		return (T_PSEUDO);
	if (s.length() == 1 && !std::isdigit(s[0]) && std::isprint(s[0]))
		return (T_CHAR);

	size_t i = 0;
	
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (i >= s.length() || !std::isdigit(s[i]))
		return (T_INVALID);

	bool hasDot = false;

	for (; i < s.length(); i++)
	{
		if (s[i] == '.')
		{
			if (hasDot)
				return (T_INVALID);
			hasDot = true;
		}
		else if (s[i] == 'f' && i == s.length() - 1)
		{
			if (hasDot)
				return (T_FLOAT);
			else
				return (T_INVALID);
		}
		else if (!std::isdigit(s[i]))
			return (T_INVALID);
	}
	if (hasDot)
		return (T_DOUBLE);
	else
		return (T_INT);
}

static void printChar(double value)
{
	if (std::isnan(value) || std::isinf(value) || value < 0 || value > 127)
		std::cout << "char: impossible" << std::endl;
	else if (!std::isprint(static_cast<int>(value)))
		std::cout << "char: Non displayable" << std::endl;
	else
		std::cout << "char: '" << static_cast<char>(value) << "'" << std::endl;
}

static void printInt(double value)
{
	if (std::isnan(value) || std::isinf(value)
		|| value > static_cast<double>(INT_MAX)
		|| value < static_cast<double>(INT_MIN))
		std::cout << "int: impossible" << std::endl;
	else
		std::cout << "int: " << static_cast<int>(value) << std::endl;
}

static void printFloat(double value, const std::string &literal)
{
	float fval = static_cast<float>(value);

	if (std::isnan(fval))
	{
		if (literal[0] == '-')
			std::cout << "float: -nanf" << std::endl;
		else
			std::cout << "float: nanf" << std::endl;
	}
	else if (std::isinf(fval))
	{
		if (literal[0] == '-')
			std::cout << "float: -inff" << std::endl;
		else
			std::cout << "float: +inff" << std::endl;
	}
	else
	{
		std::cout << std::fixed << std::setprecision(1);
		std::cout << "float: " << fval << "f" << std::endl;
	}
}

static void printDouble(double value, const std::string &literal)
{
	if (std::isnan(value))
	{
		if (literal[0] == '-')
			std::cout << "double: -nan" << std::endl;
		else
			std::cout << "double: nan" << std::endl;
	}
	else if (std::isinf(value))
	{
		if (literal[0] == '-')
			std::cout << "double: -inf" << std::endl;
		else
			std::cout << "double: +inf" << std::endl;
	}
	else
	{
		std::cout << std::fixed << std::setprecision(1);
		std::cout << "double: " << value << std::endl;
	}
}

void ScalarConverter::convert(const std::string &literal)
{
	int    type = getType(literal);
	double value = 0.0;

	if (type == T_INVALID)
	{
		std::cout << "Invalid literal" << std::endl;
		return ;
	}
	if (type == T_CHAR)
		value = static_cast<double>(literal[0]);
	else if (type == T_PSEUDO)
	{
		if (literal == "nan" || literal == "nanf")
			value = NAN;
		else if (literal == "+inf" || literal == "+inff")
			value = INFINITY;
		else
			value = -INFINITY;
	}
	else
		value = std::strtod(literal.c_str(), NULL);
	printChar(value);
	printInt(value);
	printFloat(value, literal);
	printDouble(value, literal);
}