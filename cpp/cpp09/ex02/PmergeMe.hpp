/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 10:41:45 by damalca           #+#    #+#             */
/*   Updated: 2026/04/20 12:08:27 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <exception>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <utility>
#include <iomanip>
#include <ctime>
#include <algorithm>

class PmergeMe
{
	private:
		std::vector<int>	_vec;
		std::deque<int>		_deq;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &src);
		PmergeMe &operator=(const PmergeMe &rdm);
		~PmergeMe();

		void execute(int ac, char **av);
		void parse_and_push_entries(int ac, char **av);

	class ExceptionInvalidInput : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return ("Error");
			}
	};
};