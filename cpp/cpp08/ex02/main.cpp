/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 11:31:02 by damalca           #+#    #+#             */
/*   Updated: 2026/03/27 12:34:37 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MutantStack.hpp"
#include <iostream>
#include <list>

int main()
{
	std::cout << "\n=== TESTS DU MAIN ===" << std::endl;
	
	MutantStack<int> mstack;
	
	mstack.push(5);
	mstack.push(17);
	
	std::cout << mstack.top() << std::endl;
	
	mstack.pop();
	
	std::cout << mstack.size() << std::endl;
	
	mstack.push(3);
	mstack.push(5);
	mstack.push(737);
	mstack.push(0);
	
	MutantStack<int>::iterator it = mstack.begin();
	MutantStack<int>::iterator ite = mstack.end();
	
	++it;
	--it;
	
	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}
	std::stack<int> s(mstack);

	std::cout << "\n=== TESTS OF STDLIST(should be the same) ===" << std::endl;

	std::list<int> lst;
	
	lst.push_back(5);
	lst.push_back(17);
	
	std::cout << lst.back() << std::endl;
	
	lst.pop_back();
	
	std::cout << lst.size() << std::endl;
	
	lst.push_back(3);
	lst.push_back(5);
	lst.push_back(737);
	lst.push_back(0);
	
	std::list<int>::iterator lit = lst.begin();
	std::list<int>::iterator lite = lst.end();
	
	++lit;
	--lit;
	
	while (lit != lite)
	{
		std::cout << *lit << std::endl;
		++lit;
	}

	std::cout << "\n=== ADDITIONAL TESTS ===" << std::endl;

	std::cout << "\n- Copy constructor and assignment -" << std::endl;

	MutantStack<int> copied(mstack);
	MutantStack<int> assigned;
	assigned = mstack;
	
	std::cout << "copied size   : " << copied.size() << std::endl;
	std::cout << "assigned size : " << assigned.size() << std::endl;
	std::cout << "copied top    : " << copied.top() << std::endl;
	std::cout << "assigned top  : " << assigned.top() << std::endl;

	std::cout << "\n- Const iterator test -" << std::endl;
	
	const MutantStack<int> constStack(mstack);
	MutantStack<int>::const_iterator cit = constStack.begin();
	MutantStack<int>::const_iterator cite = constStack.end();
	while (cit != cite)
	{
		std::cout << *cit << std::endl;
		++cit;
	}

	std::cout << "\n- Reverse iterator test -" << std::endl;
	MutantStack<int>::reverse_iterator rit = mstack.rbegin();
	MutantStack<int>::reverse_iterator rite = mstack.rend();
	while (rit != rite)
	{
		std::cout << *rit << std::endl;
		++rit;
	}

	std::cout << "\n--- Empty stack ---" << std::endl;
	MutantStack<int> empty;
	if (empty.begin() == empty.end())
		std::cout << "empty begin == end\n" << std::endl;

}