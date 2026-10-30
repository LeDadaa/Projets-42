/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:21:05 by damalca           #+#    #+#             */
/*   Updated: 2025/08/13 13:34:43 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "Libft/libft.h"

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_stack
{
	t_node	*head;
	t_node	*tail;
	size_t	size;
}				t_stack;

// parsing
void	solve_one(char *av);
void	solve_multiple(int ac, char **av);
void	check_one(char **tab);
void	check_multiple(int ac, char **av);

// free
void	free_tab(char **tab);
void	free_stack(t_stack *stack);

// insertion listes chainees
void	init_stack_params(t_stack *stack);
int		init_stack(t_stack *stack, int ac, char **av);

// operations
void	ra(t_stack *a);
void	pb(t_stack *a, t_stack *b);
void	pa(t_stack *a, t_stack *b);
void	rra(t_stack *a);
void	sa(t_stack *a);

// sort
void	assign_index(t_stack *a);
void	radix_sort(t_stack *a, t_stack *b);
void	sort(t_stack *a, t_stack *b);
int		is_sorted(t_stack *a);
void	sort_under_ten(t_stack *a, t_stack *b);

#endif