/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_handle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/04 15:18:36 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_space(char c)
{
	if (c && (c == ' ' || c == '\n' || c == '\r' || c == '\f'
			|| c == '\t' || c == '\v'))
		return (1);
	return (0);
}

int	handle_special(char *str)
{
	if (!str || !*str)
		return (0);
	if (ft_strlen(str) >= 2)
	{
		if (!ft_strncmp(str, "<<", 2))
			return (HEREDOC);
		if (!ft_strncmp(str, ">>", 2))
			return (APPEND);
	}
	if (str[0] == '<')
		return (REDIR_IN);
	if (str[0] == '>')
		return (REDIR_OUT);
	if (str[0] == '|')
		return (PIPE);
	return (0);
}

t_token	*new_token(char *value, int type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(value);
	if (!tok->value)
	{
		free(tok);
		return (NULL);
	}
	tok->type = type;
	tok->next = NULL;
	return (tok);
}

void	add_token(t_token **head, t_token *tok)
{
	t_token	*curr;

	if (!*head)
	{
		*head = tok;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = tok;
}
