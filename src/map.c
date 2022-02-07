/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <mbueno-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:05:52 by mbueno-g          #+#    #+#             */
/*   Updated: 2022/02/07 22:55:00 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	check_textures(char *trim, t_game *g)
{
	char	**dir;
	int		w;

	dir = ft_split(trim, ' ');
	ft_putmatrix_fd(dir, 1, 1);
	if (!dir)
	{
		free(trim);
		cub_perror(no_memory, g, NULL, 1);
	}
	else if (!ft_strncmp(dir[0], "NO", 3))
		g->tex.n = mlx_xpm_file_to_image(g->mlx_ptr, dir[1], &w, &w);
	else if (!ft_strncmp(dir[0], "SO", 3))
		g->tex.s = mlx_xpm_file_to_image(g->mlx_ptr, dir[1], &w, &w);
	else if (!ft_strncmp(dir[0], "WE", 3))
		g->tex.w = mlx_xpm_file_to_image(g->mlx_ptr, dir[1], &w, &w);
	else if (!ft_strncmp(dir[0], "EA", 3))
		g->tex.e = mlx_xpm_file_to_image(g->mlx_ptr, dir[1], &w, &w);
	else if (!ft_strncmp(dir[0], "F", 2) || !ft_strncmp(dir[0], "C", 2))
		get_cf_color(dir, g);
	ft_free_matrix(&dir);
}

void	read_map(char *file, t_game *g)
{
	char	*line[2];
	int		i;

	i = 0;
	g->fd = open(file, O_RDONLY);
	cub_perror(inv_file, g, file, g->fd < 0);
	while (1)
	{
		line[0] = get_next_line(g->fd);
		if (!line[0])
			break ;
		line[1] = ft_strtrim(line[0], "\n");
		free(line[0]);
		if ((int)ft_strlen(line[1]) > g->width)
			g->width = ft_strlen(line[1]);
		if (line[1] && line[1][0] && ++i < 7)
			check_textures(line[1], g);
		else if ((line[1] && line[1][0]) || i >= 7)
			g->map = ft_extend_matrix(g->map, line[1]);
		free(line[1]);
	}
	cub_perror(empty_file, g, NULL, !i);
	g->height = ft_matrixlen(g->map);
	cub_perror(inv_tex, g, NULL, !(!g->tex.n || !g->tex.s || \
		!g->tex.e || !g->tex.w || g->tex.floor == -1 || g->tex.ceiling));
}

void	check_characters(int j, t_game *g)
{
	int	i;

	i = -1;
	while (++i < (int) ft_strlen(g->map[j]))
	{
		if (g->pl.dir == 'M' && ft_strchr("NSWE", g->map[j][i]))
		{
			g->pl.dir = g->map[j][i];
			g->pl.x = (float) i;
			g->pl.y = (float) j;
		}
		else if (g->pl.dir != 'M' && ft_strchr("NSWE", g->map[j][i]))
			cub_perror(inv_player, g, NULL, 1);
		else if (!ft_strchr("01", g->map[j][i]))
			cub_perror(inv_charac, g, NULL, 1);
	}
	if (j == g->height - 1 && g->pl.dir == 'M')
		cub_perror(inv_player, g, NULL, 1);
}

void	check_map(char *file, t_game *g)
{
	int	j;
	int	i;
	int	w;

	read_map(file, g);
	j = 0;
	while (j < g->height)
	{
		w = ft_strlen(g->map[j]);
		i = 0;
		if (ft_strncmp(g->map[j], "", 1) == 0)
			cub_perror(inv_map, g, NULL, 1);
		while (ft_isspace(g->map[j][i]) && i < w)
			i++;
		if ((j == 0 || j == g->height - 1) && w - i - \
			ft_countchar(g->map[j], '1'))
			cub_perror(inv_wall, g, NULL, 1);
		else if (g->map[j][i] != '1' || g->map[j][w - 1] != '1')
			cub_perror(inv_wall, g, NULL, 1);
		check_characters(j, g);
		j++;
	}
	cub_perror(inv_map, g, NULL, !j);
}
