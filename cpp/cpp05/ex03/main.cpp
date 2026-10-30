/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 11:03:02 by marvin            #+#    #+#             */
/*   Updated: 2026/03/18 15:16:59 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Intern.hpp"
#include "Bureaucrat.hpp"
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(NULL));
    Intern Random;
    AForm* form;

    form = Random.makeForm("shrubbery creation", "Bender");
    if (form)
    {
        std::cout << *form << std::endl;
        delete form;
    }

    form = Random.makeForm("robotomy request", "Bender");
    if (form)
    {
        std::cout << *form << std::endl;
        delete form;
    }

    form = Random.makeForm("presidential pardon", "Bender");
    if (form)
    {
        std::cout << *form << std::endl;
        delete form;
    }
    
    form = Random.makeForm("unknown form", "Target");
    if (!form)
        std::cout << "Form creation failed as expected." << std::endl;

    form = Random.makeForm("presidential pardon", "Jack");
    if (form)
    {
        Bureaucrat president("President", 1);
        president.signForm(*form);
        president.executeForm(*form);
        delete form;
    }
    return 0;
}