/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:06:20 by damalca          #+#    #+#             */
/*   Updated: 2025/11/25 00:31:18 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	process_export_arg(char *arg, char ***env)
{
	char	*eq_sign;
	char	*name;
	char	*value;

	eq_sign = ft_strchr(arg, '=');
	if (eq_sign)
	{
		name = ft_substr(arg, 0, eq_sign - arg);
		value = ft_strdup(eq_sign + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
	}
	if (!is_valid_variable(name))
		return (export_error(arg), free(name), free(value), 1);
	if (value)
		update_or_add_env(env, name, value);
	free(name);
	free(value);
	return (0);
}

int	ft_export(char **args, char ***env)
{
	int	i;
	int	ret;

	ret = 0;
	if (!args[1])
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], env))
			ret = 1;
		i++;
	}
	return (ret);
}
