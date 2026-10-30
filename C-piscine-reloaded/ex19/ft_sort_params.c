/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:52:55 by damalca           #+#    #+#             */
/*   Updated: 2025/04/24 12:50:07 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);

int	ft_strcmp(char *s1, char *s2)
{
	int	x;

	x = 0;
	while ((s1[x] || s2[x]) && (s1[x] == s2[x]))
		x++;
	return (s1[x] - s2[x]);
}

void	ft_ascii(char **arg)
{
	int		x;
	char	*temp;

	if (!arg)
		return ;
	x = 0;
	while (arg[x + 1])
	{
		if (ft_strcmp(arg[x], arg[x + 1]) > 0)
		{
			temp = arg[x];
			arg[x] = arg[x + 1];
			arg[x + 1] = temp;
			x = 0;
		}
		else
			x++;
	}
}

int	main(int ac, char **av)
{
	int	x;
	int	y;

	if (ac > 1)
	{
		ft_ascii(&av[1]);
		x = 1;
		while (x < ac)
		{
			y = 0;
			while (av[x][y])
				ft_putchar(av[x][y++]);
			ft_putchar('\n');
			x++;
		}
	}
}
