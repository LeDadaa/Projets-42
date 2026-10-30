/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phonebook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:43:00 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 06:40:05 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phonebook.hpp"

PhoneBook::PhoneBook()
{
	_nb_contacts = 0;
	_oldest_contact = 0;
}	

std::string trim(const std::string &str)
{
	int start = 0;
	int end = str.length();

	while (start < end && (str[start] == ' ' || str[start] == '\t'))
		start++;
	while (end < start && (str[end - 1] == ' ' || str[end - 1] == '\t'))
		end--;
	
	return (str.substr(start, end - start));
}

void PhoneBook::add_contact()
{
    std::string info[5];
    std::cout << "Enter first name: ";
    std::getline(std::cin, info[0]);
    info[0] = trim(info[0]);
    if (std::cin.eof())
        return ;
    std::cout << "Enter last name: ";
    std::getline(std::cin, info[1]);
    info[1] = trim(info[1]);
    if (std::cin.eof())
        return ;
    std::cout << "Enter nickname: ";
    std::getline(std::cin, info[2]);
    info[2] = trim(info[2]);
    if (std::cin.eof())
        return ;
    std::cout << "Enter phone number: ";
    std::getline(std::cin, info[3]);
    info[3] = trim(info[3]);
    if (std::cin.eof())
        return ;
    std::cout << "Enter darkest secret: ";
    std::getline(std::cin, info[4]);
    info[4] = trim(info[4]);
    if (std::cin.eof())
        return ;

    _contacts[_oldest_contact].init_contact(info);
    _oldest_contact = (_oldest_contact + 1) % 8;
    if (_nb_contacts != 8)
        _nb_contacts++;
}

std::string truncate(const std::string& str)
{
	if (str.length() > 9)
		return (str.substr(0, 9) + ".");
	return (str);
}

void PhoneBook::search_contact()
{
	if (_nb_contacts == 0)
	{
		std::cout << "No contacts in Phonebook";
		return ;
	}
	std::cout << std::setw(10) << "Index" << "|"
			  << std::setw(10) << "First name" << "|"
			  << std::setw(10) << "Last name" << "|"
			  << std::setw(10) << "Nickname" << std::endl;

	for (int i = 0; i < _nb_contacts; ++i)
	{
		std::cout << std::setw(10) << i << "|"
				  << std::setw(10) << truncate(_contacts[i].get_first_name()) << "|"
				  << std::setw(10) << truncate(_contacts[i].get_last_name()) << "|"
				  << std::setw(10) << truncate(_contacts[i].get_nickname()) << std::endl;
	}
	std::string input;
	int index = -1;
	std::cout << "Enter the index of the contact you want to view: ";
	std::getline(std::cin, input);
	if (std::cin.eof())
		return ;
	if (input.length() == 1 && std::isdigit(input[0]))
		index = input[0] - 48;
	if (index < 0 || index >= _nb_contacts)
	{
		std::cout << "Invalid index, please try again: " << std::endl;
		return ;
	}
	std::cout << "First name: " << _contacts[index].get_first_name() << std::endl;
	std::cout << "Last name: " << _contacts[index].get_last_name() << std::endl;
	std::cout << "Nickname: " << _contacts[index].get_nickname() << std::endl;
	std::cout << "Phone number: " << _contacts[index].get_phone_number() << std::endl;
	std::cout << "Darkest secret: " << _contacts[index].get_darkest_secret() << std::endl;
}