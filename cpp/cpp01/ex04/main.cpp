/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 10:42:30 by damalca           #+#    #+#             */
/*   Updated: 2025/11/26 04:39:50 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

int main(int ac, char **av)
{
	if (ac == 4)
	{
		std::string filename = av[1];
		std::string to_find = av[2];
		if (to_find.length() < 1)
		{
			std::cout << "Error: string to replace can't be empty" << std::endl;
			return (1);
		}
		std::string to_replace = av[3];
		std::fstream infile, outfile;
		std::string output = filename + ".replace";
		std::string line, file1;
		infile.open(filename.c_str(), std::ios::in);
		if (!infile)
		{
			std::cout << "Infile open error" << std::endl;
			return (1);
		}
		while (std::getline(infile, line))
			file1 += line + '\n';
		if (file1.size() <= 0)
		{
			std::cout << "File1 error: no content found" << std::endl;
			return (1);
		}
		size_t pos = file1.find(to_find);
		while (pos != std::string::npos)
		{
			file1.erase(pos, to_find.length());
			file1.insert(pos, to_replace);
			pos = file1.find(to_find, pos + to_replace.length());
		}
		outfile.open(output.c_str(), std::ios::out);
		outfile << file1;
		return (0);
	}
}