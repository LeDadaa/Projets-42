/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 08:58:36 by damalca           #+#    #+#             */
/*   Updated: 2026/04/02 11:40:35 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() 
{
	_loadDatabase("data.csv");
}

BitcoinExchange::BitcoinExchange(const std::string& dbPath) 
{
	_loadDatabase(dbPath);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) 
{
	*this = src;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& rdm) 
{
	if (this != &rdm) 
		this->_database = rdm._database;
	return *this;
}

BitcoinExchange::~BitcoinExchange() 
{
}

void BitcoinExchange::_loadDatabase(const std::string& dbPath) 
{
	std::ifstream file(dbPath.c_str());
	
	if (!file.is_open()) 
	{
		std::cerr << "Error: could not open database file " << dbPath << std::endl;
		exit (1);
	}

	std::string line;

	if (std::getline(file, line) && line != "date,exchange_rate") 
	{
		file.clear();
		file.seekg(0, std::ios::beg);
	}
	while (std::getline(file, line)) 
	{
		if (line.empty()) 
			continue ;

		size_t commaPos = line.find(",");
		if (commaPos == std::string::npos) 
			continue ;

		std::string dateStr = line.substr(0, commaPos);
		std::string valueStr = line.substr(commaPos + 1);
		float value = 0.0f;
		
		if (_isValidDate(dateStr) && _isValidValue(valueStr, value, false)) 
			_database[dateStr] = value;
	}
}

bool BitcoinExchange::_isValidDate(const std::string& date) const 
{
	if (date.length() != 10)
		return (false);
	if (date[4] != '-' || date[7] != '-')
		return (false);

	for (unsigned int i = 0; i < date.length(); ++i) 
	{
		if (i == 4 || i == 7) 
			continue ;
		if (!isdigit(date[i])) 
			return (false);
	}
	
	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (year < 2009 || month < 1 || month > 12 || day < 1 || day > 31)
		return (false);
	if (day == 31 && (month == 2 || month == 4 || month == 6 || month == 9 || month == 11))
		return (false);
	if (month == 2) 
	{
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (day > 29 || (!isLeap && day == 29))
			return (false);
	}
	return (true);
}

bool BitcoinExchange::_isValidValue(const std::string& valueStr, float& value, bool isInput) const 
{
	char *endptr;
	double val = std::strtod(valueStr.c_str(), &endptr);


	if (valueStr.empty() || (*endptr != '\0' && *endptr != '\n' && *endptr != '\r')) 
	{
		if (isInput) std::cerr << "Error: invalid number format." << std::endl;
		return (false);
	}
	if (val < 0) 
	{
		if (isInput) std::cerr << "Error: not a positive number." << std::endl;
		return (false);
	}
	if (isInput && val > 1000) 
	{
		std::cerr << "Error: too large a number." << std::endl;
		return (false);
	}
	value = static_cast<float>(val);
	return (true);
}

float BitcoinExchange::_getExchangeRate(const std::string& date) const 
{
	if (_database.empty())
		return (0.0f);

	std::map<std::string, float>::const_iterator it = _database.lower_bound(date);

	if (it != _database.end() && it->first == date)
		return (it->second);
	if (it == _database.begin())
		return (it->second);
	--it;
	return (it->second);
}

void BitcoinExchange::processInputFile(const std::string& inputPath) const 
{
	std::ifstream file(inputPath.c_str());
	if (!file.is_open()) 
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	if (std::getline(file, line) && line != "date | value") 
	{
		file.clear();
		file.seekg(0, std::ios::beg);
	}
	while (std::getline(file, line)) 
	{
		if (line.empty())
			continue ;

		size_t pipePos = line.find(" | ");
		if (pipePos == std::string::npos) 
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue ;
		}

		std::string dateStr = line.substr(0, pipePos);
		std::string valueStr = line.substr(pipePos + 3);
		float value = 0.0f;

		if (!_isValidDate(dateStr))
			std::cerr << "Error: bad input => " << dateStr << std::endl;
		else if (!_isValidValue(valueStr, value, true)) 
		{
		} 
		else 
		{
			float rate = _getExchangeRate(dateStr);
			std::cout << dateStr << " => " << value << " = " << (value * rate) << std::endl;
		}
	}
}
