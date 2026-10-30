/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Intern.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 12:32:52 by damalca           #+#    #+#             */
/*   Updated: 2026/01/29 12:42:45 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"

class Intern
{
    private:
        AForm* createShrubberyForm(const std::string& target);
        AForm* createRobotomyForm(const std::string& target);
        AForm* createPresidentialForm(const std::string& target);

    public:
        Intern();
        ~Intern();
        Intern(const Intern& random);
        Intern& operator=(const Intern& src);

        AForm* makeForm(const std::string& formName, const std::string& target);
};