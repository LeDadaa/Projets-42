/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 14:03:52 by marvin            #+#    #+#             */
/*   Updated: 2026/01/29 10:03:49 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Form.hpp"

Form::Form() : _name("Default"), _isSigned(false), _gradeToSign(150), 
				_gradeToExecute(150) {}

Form::~Form() {}

Form::Form(const std::string &name, const int gradeToSign, 
			const int gradeToExecute) : _name(name), _isSigned(false),
			 _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute) 
{
	if (gradeToSign < 1 || gradeToExecute < 1)
		throw GradeTooHighException();
	if (gradeToSign > 150 || gradeToExecute > 150)
		throw GradeTooLowException();
}

Form::Form(const Form &random) : _name(random._name), _isSigned(random._isSigned),
			 _gradeToSign(random._gradeToSign), _gradeToExecute(random._gradeToExecute) {}
			 
Form &Form::operator=(const Form &src) 
{
	_isSigned = src._isSigned;
	return (*this);
}

void Form::beSigned(const Bureaucrat &bureaucrat)
{
	if (_isSigned)
        throw std::logic_error("Form is already signed");
    if (bureaucrat.getGrade() > _gradeToSign)
        throw GradeTooLowException();
    _isSigned = true;
}

std::string Form::getName() const
{
	return (_name);
}

bool Form::getIsSigned() const
{
	return (_isSigned);
}

int Form::getGradeToSign() const
{
	return (_gradeToSign);
}

int Form::getGradeToExecute() const
{
	return (_gradeToExecute);
}

const char *Form::GradeTooHighException::what() const throw()
{
	return ("Grade too high!");
}

const char *Form::GradeTooLowException::what() const throw()
{
	return ("Grade too low!");
}

std::ostream &operator<<(std::ostream &os,  Form const &form)
{
	os << "Name: " << form.getName() << " isSigned: " << form.getIsSigned() << " SignGrade: "
		<< form.getGradeToSign() << " ExecuteGrade: " << form.getGradeToExecute() << std::endl;
	return (os);
}