/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/04 15:18:59 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_args(t_token *start, t_token *end)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = start;
	while (curr && curr != end)
	{
		if (curr->type == ARG)
		{
			count++;
			curr = curr->next;
		}
		else if (curr->type != PIPE)
		{
			curr = curr->next;
			if (curr && curr != end)
				curr = curr->next;
		}
		else
			curr = curr->next;
	}
	return (count);
}

static t_token	*skip_tokens(t_token *curr, t_token *end)
{
	if (curr->type != ARG && curr->type != PIPE)
	{
		curr = curr->next;
		if (curr && curr != end)
			curr = curr->next;
	}
	else
		curr = curr->next;
	return (curr);
}

char	**add_args(t_token *start, t_token *end)
{
	char	**args;
	int		i;
	t_token	*curr;

	args = malloc(sizeof(char *) * (count_args(start, end) + 1));
	if (!args)
		return (NULL);
	i = 0;
	curr = start;
	while (curr && curr != end)
	{
		if (curr->type == ARG)
			args[i++] = ft_strdup(curr->value);
		curr = skip_tokens(curr, end);
	}
	args[i] = NULL;
	return (args);
}
