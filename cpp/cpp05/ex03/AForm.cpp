/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 14:03:52 by marvin            #+#    #+#             */
/*   Updated: 2026/01/29 12:27:33 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AForm.hpp"

AForm::AForm() : _name("Default"), _isSigned(false), _gradeToSign(150), 
				_gradeToExecute(150) {}

AForm::~AForm() {}

AForm::AForm(const std::string &name, const int gradeToSign, 
			const int gradeToExecute) : _name(name), _isSigned(false),
			 _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute) 
{
	if (gradeToSign < 1 || gradeToExecute < 1)
		throw GradeTooHighException();
	if (gradeToSign > 150 || gradeToExecute > 150)
		throw GradeTooLowException();
}

AForm::AForm(const AForm &random) : _name(random._name), _isSigned(random._isSigned),
			 _gradeToSign(random._gradeToSign), _gradeToExecute(random._gradeToExecute) {}
			 
AForm &AForm::operator=(const AForm &src) 
{
	_isSigned = src._isSigned;
	return (*this);
}

void AForm::beSigned(const Bureaucrat &bureaucrat)
{
	if (_isSigned)
        throw std::logic_error("Form is already signed");
    if (bureaucrat.getGrade() > _gradeToSign)
        throw GradeTooLowException();
    _isSigned = true;
}

std::string AForm::getName() const
{
	return (_name);
}

bool AForm::getIsSigned() const
{
	return (_isSigned);
}

int AForm::getGradeToSign() const
{
	return (_gradeToSign);
}

int AForm::getGradeToExecute() const
{
	return (_gradeToExecute);
}

const char *AForm::GradeTooHighException::what() const throw()
{
	return ("Grade too high!");
}

const char *AForm::GradeTooLowException::what() const throw()
{
	return ("Grade too low!");
}

const char *AForm::FormNotSignedException::what() const throw()
{
    return ("Form is not signed!");
}

void AForm::execute(Bureaucrat const & executor) const
{
	if (!_isSigned)
		throw FormNotSignedException();
	if (executor.getGrade() > _gradeToExecute)
		throw GradeTooLowException();
	doAction();
}

std::ostream &operator<<(std::ostream &os, AForm const &form)
{
	os << "Name: " << form.getName() << " isSigned: " << form.getIsSigned() << " SignGrade: "
		<< form.getGradeToSign() << " ExecuteGrade: " << form.getGradeToExecute() << std::endl;
	return (os);
}