/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:55:53 by damalca           #+#    #+#             */
/*   Updated: 2025/08/04 12:10:20 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "Libft/libft.h"
# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"
# include <X11/X.h>
# include <X11/keysym.h>

# define T_S 100

typedef struct s_game
{
	char	**map;
	int		w;
	int		h;

	void	*mlx;
	void	*win;

	void	*pl;
	void	*fl;
	void	*col;
	void	*wa;
	void	*ex;

	int		pl_x;
	int		pl_y;

	int		moves;

	int		c;
	int		t_c;

	int		keymode;
}			t_game;

char	**game_map(char *file);
void	map_generation(t_game *g);
void	put_img(t_game *g, void *img, int x, int y);
int		handle_moves(int keycode, t_game *g);
int		close_window(t_game *g);
int		check_args(int ac, char **av);
int		check_if_exists(char **map);
int		check_if_rectangle(char **map);
int		check_duplicates(char **map);
int		check_chars(char **map);
int		check_walls(char **map);
int		check_if_one_line(char **map);
int		check_map(char **map);
int		check_path(char **map);
int		check_if_collectible(char **map);
int		check_if_exit(char **map);
int		check_if_player(char **map);
void	free_game(t_game *g);
int		check_if_too_big(char **map);
void	init_game_struct(t_game *g);
int		init_map(t_game *g, char *file);
void	init_game(t_game *g);
void	init_textures(t_game *g);
void	init_size(t_game *g);
void	init_game_struct(t_game *g);

#endif