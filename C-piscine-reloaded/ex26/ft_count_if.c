/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_if.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:13:35 by damalca           #+#    #+#             */
/*   Updated: 2025/04/25 09:47:31 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_count_if(char **tab, int (*f)(char*))
{
	int	x;
	int	count;

	if (!tab || !f)
		return (0);
	x = 0;
	count = 0;
	while (tab[x])
	{
		if (f(tab[x++]) == 1)
			count++;
	}
	return (count);
}
