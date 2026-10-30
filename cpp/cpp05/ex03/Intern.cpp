/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Intern.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 12:33:00 by damalca           #+#    #+#             */
/*   Updated: 2026/01/29 13:08:50 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Intern.hpp"
#include <iostream>

Intern::Intern() {}

Intern::~Intern() {}

Intern::Intern(const Intern &random)
{
	(void)random;
}

Intern &Intern::operator=(const Intern& src)
{
	(void)src;
	return (*this);
}

AForm* Intern::createPresidentialForm(const std::string& target)
{
    return new PresidentialPardonForm(target);
}

AForm* Intern::createRobotomyForm(const std::string& target)
{
    return new RobotomyRequestForm(target);
}
AForm* Intern::createShrubberyForm(const std::string& target)
{
    return new ShrubberyCreationForm(target);
}

AForm* Intern::makeForm(const std::string& formName, const std::string& target)
{
    std::string formNames[3] = {
        "shrubbery creation",
        "robotomy request",
        "presidential pardon"
    };
    AForm* (Intern::*formCreators[3])(const std::string&) = {
        &Intern::createShrubberyForm,
        &Intern::createRobotomyForm,
        &Intern::createPresidentialForm
    };
    for (int i = 0; i < 3; i++)
    {
        if (formName == formNames[i])
        {
            std::cout << "Intern creates " << formName << std::endl;
            return (this->*formCreators[i])(target);
        }
	}
    std::cerr << "Error: Form \"" << formName << "\" does not exist!" << std::endl;
    return NULL;
}
