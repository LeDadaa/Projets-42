/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:42:49 by damalca           #+#    #+#             */
/*   Updated: 2025/10/24 15:21:19 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "contact.hpp"

class PhoneBook {
	private: 
		Contact _contacts[8];
		int _nb_contacts;
		int _oldest_contact;
	public:
		void add_contact();
		void search_contact();
		PhoneBook();
};