/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 10:41:35 by damalca           #+#    #+#             */
/*   Updated: 2026/04/20 12:22:12 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &src)
{
	*this = src;
}
		
PmergeMe &PmergeMe::operator=(const PmergeMe &rhs)
{
	if (this != &rhs)
	{
		this->_vec = rhs._vec;
		this->_deq = rhs._deq;
	}
	return (*this);
}
		
PmergeMe::~PmergeMe()
{
}

void PmergeMe::parse_and_push_entries(int ac, char **av)
{
	for (int i = 0; i < ac; i++)
	{
		std::string str(av[i]);
		
		if (str.empty())
			throw (PmergeMe::ExceptionInvalidInput());

		size_t j = 0;
		
		if (str[j] == '+')
			j++;
		if (j == str.length())
			throw (PmergeMe::ExceptionInvalidInput());
		for (; j < str.length(); j++)
			if (!std::isdigit(str[j]))
				throw (PmergeMe::ExceptionInvalidInput());
		
		char *endptr;
		errno = 0;
		long val = std::strtol(av[i], &endptr, 10);

		if (errno == ERANGE || val < 0 || val > INT_MAX || *endptr != '\0')
			throw (PmergeMe::ExceptionInvalidInput());

		int number = static_cast<int>(val);
        if (std::find(_vec.begin(), _vec.end(), number) != _vec.end())
            throw (PmergeMe::ExceptionInvalidInput());

		_vec.push_back(static_cast<int>(val));
		_deq.push_back(static_cast<int>(val));
	}
}

void sortVector(std::vector<int> &array)
{
	if (array.size() <= 1)
		return;
	
	bool	arrayIsOdd = false;
	int		excludedNumber = -1;
	
	if (array.size() % 2 != 0)
	{
		arrayIsOdd = true;
		excludedNumber = array.back();
		array.pop_back();
	}

	std::vector< std::pair<int, int> > pairs;
	
	for (size_t i = 0; i < array.size(); i += 2)
	{
		int a = array[i];
		int b = array[i + 1];
		
		if (a < b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}

	for (size_t i = 1; i < pairs.size(); i++)
	{
		std::pair<int, int> tmp = pairs[i];
		int j = i - 1;
		while (j >= 0 && pairs[j].second > tmp.second)
		{
			pairs[j + 1] = pairs[j];
			j--;
		}
		pairs[j + 1] = tmp;
	}

	std::vector<int>	mainChain;
	std::vector<int>	secChain;
	
	for (size_t i = 0; i < pairs.size(); i++)
	{
		secChain.push_back(pairs[i].first);
		mainChain.push_back(pairs[i].second);
	}
	if (!secChain.empty())
		mainChain.insert(mainChain.begin(), secChain[0]);

	std::vector<int>	jacobsthalIndex;
	int 				j_prev = 1;
	int					j_curr = 3;
	int					last_inserted = 0;
	std::vector<int> 	insert_order;

	while (last_inserted < (int)secChain.size() - 1)
	{
		int j_max = j_curr;
		if (j_max > (int)secChain.size() - 1)
			j_max = secChain.size() - 1;
		
		for (int i = j_max; i > last_inserted; i--)
			insert_order.push_back(i);

		last_inserted = j_max;
		
		int	tmp = j_curr;
		j_curr = j_curr + 2 * j_prev;
		j_prev = tmp;
	}

	for (size_t i = 0; i < insert_order.size(); i++)
	{
		int	target = secChain[insert_order[i]];
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), target);
        mainChain.insert(pos, target);
	}

	if (arrayIsOdd)
    {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), excludedNumber);
        mainChain.insert(pos, excludedNumber);
    }
	array = mainChain;
}

void sortDeque(std::deque<int> &array)
{
	if (array.size() <= 1)
		return;
	
	bool	arrayIsOdd = false;
	int		excludedNumber = -1;
	
	if (array.size() % 2 != 0)
	{
		arrayIsOdd = true;
		excludedNumber = array.back();
		array.pop_back();
	}

	std::deque< std::pair<int, int> > pairs;
	
	for (size_t i = 0; i < array.size(); i += 2)
	{
		int a = array[i];
		int b = array[i + 1];
		
		if (a < b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}

	for (size_t i = 1; i < pairs.size(); i++)
	{
		std::pair<int, int> tmp = pairs[i];
		int j = i - 1;
		while (j >= 0 && pairs[j].second > tmp.second)
		{
			pairs[j + 1] = pairs[j];
			j--;
		}
		pairs[j + 1] = tmp;
	}

	std::deque<int>	mainChain;
	std::deque<int>	secChain;
	
	for (size_t i = 0; i < pairs.size(); i++)
	{
		secChain.push_back(pairs[i].first);
		mainChain.push_back(pairs[i].second);
	}
	if (!secChain.empty())
		mainChain.insert(mainChain.begin(), secChain[0]);

	std::deque<int>		jacobsthalIndex;
	int 				j_prev = 1;
	int					j_curr = 3;
	int					last_inserted = 0;
	std::deque<int> 	insert_order;

	while (last_inserted < (int)secChain.size() - 1)
	{
		int j_max = j_curr;
		if (j_max > (int)secChain.size() - 1)
			j_max = secChain.size() - 1;

		for (int i = j_max; i > last_inserted; i--)
			insert_order.push_back(i);

		last_inserted = j_max;
		
		int	tmp = j_curr;
		j_curr = j_curr + 2 * j_prev;
		j_prev = tmp;
	}

	for (size_t i = 0; i < insert_order.size(); i++)
	{
		int	target = secChain[insert_order[i]];
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), target);
        mainChain.insert(pos, target);
	}

	if (arrayIsOdd)
    {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), excludedNumber);
        mainChain.insert(pos, excludedNumber);
    }
	array = mainChain;
}

void PmergeMe::execute(int ac, char **av)
{
	parse_and_push_entries(ac, av);

        std::cout << "Before: ";
        for (size_t i = 0; i < _vec.size(); ++i) {
                if (i == 5) { std::cout << "[...] "; break; }
                std::cout << _vec[i] << " ";
        }
        std::cout << std::endl;

	std::clock_t start_vec = std::clock();
	sortVector(_vec);
	std::clock_t end_vec = std::clock();

	std::clock_t start_deq = std::clock();
	sortDeque(_deq);
	std::clock_t end_deq = std::clock();

        std::cout << "After:  ";
        for (size_t i = 0; i < _vec.size(); ++i) 
		{
                if (i == 5) 
				{ 
					std::cout << "[...] "; 
					break; 
				}
                std::cout << _vec[i] << " ";
        }
        std::cout << std::endl;

	double time_vec = 1000000.0 * (end_vec - start_vec) / CLOCKS_PER_SEC;
    double time_deq = 1000000.0 * (end_deq - start_deq) / CLOCKS_PER_SEC;

    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << time_vec << " us" << std::endl;
              
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque  : " 
              << std::fixed << std::setprecision(5) << time_deq << " us" << std::endl;
}