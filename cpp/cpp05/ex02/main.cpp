/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 11:03:02 by marvin            #+#    #+#             */
/*   Updated: 2026/03/18 15:13:34 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <cstdlib>
#include <ctime>

int main()
{
	srand(time(NULL));

	Bureaucrat boss("Boss", 1);
	ShrubberyCreationForm shrubbery("mon jardin");
	RobotomyRequestForm robotomy("Bender");
	PresidentialPardonForm pardon("Arthur");

	std::cout << boss << std::endl;
	std::cout << shrubbery;
	std::cout << robotomy;
	std::cout << pardon;

	boss.signForm(shrubbery);
	boss.signForm(robotomy);
	boss.signForm(pardon);

	boss.executeForm(shrubbery);
	boss.executeForm(robotomy);
	boss.executeForm(robotomy);
	boss.executeForm(pardon);

	Bureaucrat alice("Alice", 1);
	ShrubberyCreationForm notSigned("unsigned");
	alice.executeForm(notSigned); // should fail

	Bureaucrat lowSigner("LowSigner", 150);
	PresidentialPardonForm hardForm("Ford Prefect");
	lowSigner.signForm(hardForm); //should fail

	Bureaucrat signer("Signer", 40);
	Bureaucrat lowExecutor("LowExecutor", 140);
	RobotomyRequestForm target("Marvin");

	signer.signForm(target);
	lowExecutor.executeForm(target); // should fail

	return 0;
}