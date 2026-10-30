/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 11:03:02 by marvin            #+#    #+#             */
/*   Updated: 2026/03/18 15:00:42 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"

int main()
{
    try {
        Form formA("Tax Form", 50, 25);
        std::cout << formA << std::endl;
        Form formB("Secret Document", 1, 1);
        std::cout << formB << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try {
        Form invalid("Invalid", 0, 50); // should fail
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try {
        Form invalid("Invalid", 50, 151); // should fail
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try {
        Bureaucrat bob("Bob", 45);
        Form form1("Contract", 50, 25);
        std::cout << bob << std::endl;
        std::cout << form1 << std::endl;
        bob.signForm(form1);
        std::cout << form1 << std::endl;
        bob.signForm(form1);
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try {
        Bureaucrat john("John", 100);
        Form form2("Top Secret", 50, 25);
        std::cout << john << std::endl;
        john.signForm(form2);
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}