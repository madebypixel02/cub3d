/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:25:42 by aperez-b          #+#    #+#             */
/*   Updated: 2022/02/07 12:38:31 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H

# define CUB3D_H

# include "../libft/inc/libft.h"
# include "../get_next_line/get_next_line.h"

# include <mlx.h>
# include "my_mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>

/* Enum to handle various exit/error messages */
typedef enum e_cub_err
{
	end,
	inv_argc,
	inv_ext,
	inv_file,
	empty_file,
	no_memory,
	inv_color,
	inv_wall,
	inv_map,
	inv_charac,
	inv_player,
	inv_tex
}				t_cub_err;

/* Struct to handle rgb colors with transparency */
typedef struct s_color
{
	short	t;
	short	r;
	short	g;
	short	b;
}				t_color;

/* Every image corresponding to a wall texture, plus floor and ceiling */
typedef struct s_tex
{
	void	*north;
	void	*south;
	void	*west;
	void	*east;
	int		hex_floor;
	int		hex_ceiling;
}				t_tex;

/* Struct to store various attributes of our player */
typedef struct s_player
{
	t_vector	pos;
	char		dir;
}				t_player;

/* Struct to store all ray-casting-related data */
typedef struct s_ray
{
	float	x;
	float	y;
	float	incre_angle;
	int		angle;
	int		hfov;
	int		width;
	int		height;
	int		ray_angle;
	int		precision;
}				t_ray;

/* Handles most game attributes */
typedef struct s_game
{
	int			fd;
	char		**map;
	int			height;
	int			width;
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		win_img;
	t_tex		tex;
	t_ray		ray;
	t_player	pl;
}				t_game;

/* Check possible map errors */
void	check_map(char *file, t_game *g);

/* Prints appropriate error message and exits, freeing everything */
int		cub_perror(t_cub_err err, t_game *g, char *param, int c);

/* Prints usage for the cub3D program */
void	cub_usage(int errno);

/* Atoi for colors (more restrictive than original) */
int		cub_atoi(const char *nptr, short *nbr);

/* Fills color int for floor and ceiling */
void	get_cf_color(char **dir, t_game *g);

/* Frees all necessary things before exiting */
void	cub_end(t_game *g);

/* Initializes game */
void	game_init(t_game *g);

/* Draws game */
void	draw_game(t_game *g);

/* Converts degrees to radians */
float	degree_to_radians(int degree);

#endif
