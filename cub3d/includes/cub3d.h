/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:09:42 by damalca           #+#    #+#             */
/*   Updated: 2026/03/14 15:34:45 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../minilibx/mlx.h"
# include "../Libft/libft.h"
# include "../Libft/ft_printf/ft_printf.h"
# include "../Libft/get_next_line/get_next_line.h"
# include <math.h>
# include <fcntl.h>
# include <X11/X.h>
# include <X11/keysym.h>

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
# define TEX_SIZE 64
# define MOVE_SPEED 0.05
# define ROT_SPEED 0.05
# define WALL_MARGIN 0.2
# define ESC 65307
# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define MINI_SIZE 8
# define MINI_OFFSET 10
# define MOUSE_SENSITIVITY 0.001

typedef struct s_vector
{
	double	x;
	double	y;
}	t_vector;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	encoded;
}	t_color;

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}	t_texture;

typedef struct s_player
{
	t_vector	pos;
	t_vector	dir;
	t_vector	plane;
	char		orientation;
}	t_player;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	int		parsing_map;
	char	*first_map_line;
	int		player_count;
}	t_map;

typedef struct s_config
{
	char		*no_texture;
	char		*so_texture;
	char		*we_texture;
	char		*ea_texture;
	t_color		floor;
	t_color		ceiling;
	int			textures_loaded;
}	t_config;

typedef struct s_ray
{
	t_vector	dir;
	t_vector	side_dist;
	t_vector	delta_dist;
	t_vector	map;
	int			step_x;
	int			step_y;
	int			side;
	double		perp_wall_dist;
	int			line_height;
	int			draw_start;
	int			draw_end;
	double		wall_x;
	int			tex_x;
}	t_ray;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*img_addr;
	int			bpp;
	int			line_len;
	int			endian;
	t_config	config;
	t_map		map;
	t_player	player;
	t_texture	textures[4];
	t_keys		keys;
	int			mouse_warping;
}	t_game;

/* Parsing */
int		parse_file(char *filename, t_game *game);
int		validate_map(t_game *game);
char	*trim_newline(char *str);
void	free_split(char **split);
int		check_map_chars(t_game *game);
int		is_close_by_walls(t_game *game);
int		is_map_fully_closed(t_game *game);
int		parse_texture(char **dest, char *line);
int		parse_line(char *line, t_game *game);
int		read_map_lines(int fd, t_game *game);
void	calculate_map_width(t_game *game);
void	trim_map_left(t_game *game);
int		parse_rgb(char *line, t_color *color);
int		is_map_fully_closed_supp(t_game *game, int x, int y, int width);
void	draw_rec(t_game *game, t_vector pos, int size, int color);
void	normalize_map_to_rectangle(t_game *game);
int		ft_digit(char *str);

/* Init + load etc etc */
void	init_game(t_game *game);
void	init_player(t_game *game);
void	init_player_direction(t_game *game);
int		init_mlx(t_game *game);
int		init_textures(t_game *game);
void	load_textures(t_game *game);
void	put_pixel_in_img(t_game *game, int x, int y, int color);
void	draw_mini_map(t_game *game);

/* Rendering + raycasting etc etc */
int		render_frame(t_game *game);
void	cast_ray(t_game *game, int x);
void	init_ray(t_ray *ray, t_game *game, int x);
void	algo_dda(t_ray *ray, t_game *game);
void	wall_height(t_ray *ray);
void	render_column(t_game *game, t_ray *ray, int x);
void	draw_floor_and_ceiling(t_game *game);

/* Movements + souris */
void	move_forward(t_game *game);
void	move_left(t_game *game);
void	move_backward(t_game *game);
void	move_right(t_game *game);
void	rotate_left(t_game *game);
void	rotate_right(t_game *game);
void	rotate_by_angle(t_game *game, double angle);

/* Partie 2 de mouvement + souris etc etc */
int		close_window(t_game *game);
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_game *game);
int		mouse_move(int x, int y, t_game *game);
int		game_loop(t_game *game);
void	setup_events(t_game *game);

/* Free and ft_error */
void	free_game(t_game *game);
void	ft_error(char *msg);

#endif