/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:19:27 by damalca           #+#    #+#             */
/*   Updated: 2026/03/11 11:25:46 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <climits>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <cstdlib>

#define T_CHAR     0
#define T_INT      1
#define T_FLOAT    2
#define T_DOUBLE   3
#define T_PSEUDO   4
#define T_INVALID -1

class ScalarConverter
{
	private:
		ScalarConverter();
		ScalarConverter(const ScalarConverter &src);
		ScalarConverter &operator=(const ScalarConverter &other);
		~ScalarConverter();

	public:
		static void convert(const std::string &literal);
};