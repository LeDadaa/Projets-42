/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:35:00 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 00:31:07 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	do_swap(char **sorted, int i, int j)
{
	char	*tmp;

	tmp = sorted[i];
	sorted[i] = sorted[j];
	sorted[j] = tmp;
}

static void	sort_env(char **sorted)
{
	int	i;
	int	j;

	i = -1;
	while (sorted[++i])
	{
		j = i;
		while (sorted[++j])
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
				do_swap(sorted, i, j);
		}
		printf("export %s\n", sorted[i]);
	}
}

void	print_sorted_env(char **env)
{
	int		i;
	char	**sorted;

	i = 0;
	while (env[i])
		i++;
	sorted = malloc(sizeof(char *) * (i + 1));
	if (!sorted)
		return ;
	i = -1;
	while (env[++i])
		sorted[i] = env[i];
	sorted[i] = NULL;
	sort_env(sorted);
	free(sorted);
}
