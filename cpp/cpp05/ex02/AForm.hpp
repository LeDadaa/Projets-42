/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Form.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 14:03:58 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 14:03:58 by marvin           ###   ########.fr       */ 
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Bureaucrat.hpp"
#include <stdexcept>
#include <fstream>

class Bureaucrat;

class AForm 
{
    private:
        const std::string _name;
        bool _isSigned;
        const int _gradeToSign;
        const int _gradeToExecute;

    public:
        AForm();
        virtual ~AForm();
        AForm(const std::string &name, const int gradeToSign, const int gradeToExecute);
        AForm(const AForm &random);
        AForm &operator=(const AForm &src);

        void beSigned(const Bureaucrat &bureaucrat);
        void execute(Bureaucrat const & executor) const;
        virtual void doAction() const = 0;
        std::string getName() const; 
        bool getIsSigned() const;
        int getGradeToSign() const;
        int getGradeToExecute() const;
        

        class GradeTooHighException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		
		class GradeTooLowException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

        class FormNotSignedException : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
};

std::ostream &operator<<(std::ostream &os, AForm const &form);