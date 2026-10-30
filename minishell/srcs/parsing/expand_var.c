/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/04 15:14:14 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*extract_var_name(char *str, int *len)
{
	int	i;

	i = 0;
	if (str[i] == '?')
	{
		*len = 1;
		return (ft_strdup("?"));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

void	handle_dollar(char *str, int *i, t_expand *ctx)
{
	char	*var_name;
	char	*var_value;
	int		len;

	len = 0;
	if (!is_var_start(str[*i + 1]))
	{
		append_char(&ctx->result, '$');
		(*i)++;
		return ;
	}
	(*i)++;
	var_name = extract_var_name(str + *i, &len);
	if (!var_name)
		return ;
	var_value = get_env_content(var_name, ctx->env, ctx->exit_status);
	ctx->result = strjoin_free(ctx->result, var_value);
	free(var_value);
	free(var_name);
	*i += len;
}

static void	process_quote(char *str, char *new, int i, int j)
{
	int	sq;
	int	dq;

	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !dq)
		{
			sq = !sq;
			i++;
		}
		else if (str[i] == '"' && !sq)
		{
			dq = !dq;
			i++;
		}
		else
			new[j++] = str[i++];
	}
	new[j] = '\0';
}

char	*remove_quotes_final(char *str)
{
	char	*new;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	process_quote(str, new, 0, 0);
	return (new);
}
