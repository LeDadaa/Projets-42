/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 11:24:14 by damalca           #+#    #+#             */
/*   Updated: 2026/03/20 15:56:47 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <iostream>
# include <string>

template <typename T>
void iter(T *array, const std::size_t length, void (*func)(T &))
{
	for (std::size_t i = 0; i < length; ++i)
		func(array[i]);
}

template <typename T>
void iter(T *array, const std::size_t length, void (*func)(const T &))
{
	for (std::size_t i = 0; i < length; ++i)
		func(array[i]);
}
