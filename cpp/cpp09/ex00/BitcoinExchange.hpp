/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 08:58:28 by damalca           #+#    #+#             */
/*   Updated: 2026/04/02 11:40:08 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <map>
# include <fstream>
# include <sstream>
# include <stdexcept>
# include <cstdlib>

class BitcoinExchange
{
	private:
		std::map<std::string, float> _database;

		void _loadDatabase(const std::string& dbPath);
		bool _isValidDate(const std::string& date) const;
		bool _isValidValue(const std::string& valueStr, float& value, bool isInput) const;
		float _getExchangeRate(const std::string& date) const;
	
	public:
		BitcoinExchange();
		BitcoinExchange(const std::string& dbPath);
		BitcoinExchange(const BitcoinExchange& src);
		BitcoinExchange& operator=(const BitcoinExchange& rdm);
		~BitcoinExchange();

		void processInputFile(const std::string& inputPath) const;
};