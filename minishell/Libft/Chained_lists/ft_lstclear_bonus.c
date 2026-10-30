/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:53:22 by damalca           #+#    #+#             */
/*   Updated: 2025/04/27 11:07:34 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*res;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		res = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = res;
	}
}
