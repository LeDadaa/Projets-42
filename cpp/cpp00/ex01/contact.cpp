/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:30:42 by damalca           #+#    #+#             */
/*   Updated: 2025/10/25 17:57:31 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "contact.hpp"

void Contact::init_contact(std::string info[5])
{
	std::string types[5] = { "First Name", "Last Name", "Nickname", "Phone number", "Darkest secret" };
	for (int i = 0; i < 5; i++)
	{
		while (info[i].empty())
		{
		std::cout << "This field can't be empty: " << types[i] << std::endl;
		std::getline(std::cin, info[i]);
		if (std::cin.eof())
			break ;
		}
		if (i == 0)
			_first_name = info[0];
		if (i == 1)
			_last_name = info[1];
		if (i == 2)
			_nickname = info[2];
		if (i == 3)
			_phone_number = info[3];
		if (i == 4)
			_darkest_secret = info[4];
	}
}

void Contact::display_contact()
{
	std::cout << "First Name: " << _first_name << std::endl;
	std::cout << "Last Name: " << _last_name << std::endl;
	std::cout << "Nickname: " << _nickname << std::endl;
	std::cout << "Phone Number: " << _phone_number << std::endl;
	std::cout << "Darkest Secret: " << _darkest_secret << std::endl;
}

std::string Contact::get_first_name() {return _first_name; }
std::string Contact::get_last_name() {return _last_name; }
std::string Contact::get_nickname() {return _nickname; }
std::string Contact::get_phone_number() {return _phone_number;}
std::string Contact::get_darkest_secret() {return _darkest_secret;}
