/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <mbueno-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:05:52 by mbueno-g          #+#    #+#             */
/*   Updated: 2022/02/17 08:37:10 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	get_anim(t_img *img, t_list **anim, int *num)
{
	if (num && anim && img && *anim && ft_lstsize(*anim))
		(*num)--;
	ft_lstadd_back(anim, ft_lstnew(img));
}

void	check_textures(char *trim, t_game *g, int *num)
{
	char	**dir;
	t_img	*i;

	dir = ft_split(trim, ' ');
	if (!dir)
	{
		free(trim);
		cub_perror(no_memory, g, NULL, 1);
	}
	if (ft_strncmp(dir[0], "F", 2) && ft_strncmp(dir[0], "C", 2))
		i = mlx_load_img(g->mlx_ptr, dir[1]);
	if (!ft_strncmp(dir[0], "NO", 3))
		get_anim(i, &g->tex.n, num);
	else if (!ft_strncmp(dir[0], "SO", 3))
		get_anim(i, &g->tex.s, num);
	else if (!ft_strncmp(dir[0], "EA", 3))
		get_anim(i, &g->tex.e, num);
	else if (!ft_strncmp(dir[0], "WE", 3))
		get_anim(i, &g->tex.w, num);
	else if (!ft_strncmp(dir[0], "F", 2) || !ft_strncmp(dir[0], "C", 2))
		get_cf_color(dir, g);
	ft_free_matrix(&dir);
}

void	read_map(char *file, t_game *g)
{
	char	*line[2];
	int		num;

	num = 0;
	g->fd = open(file, O_RDONLY);
	cub_perror(inv_file, g, file, g->fd < 0);
	while (1)
	{
		line[0] = get_next_line(g->fd);
		if (!line[0])
			break ;
		line[1] = ft_strtrim(line[0], "\n");
		free(line[0]);
		if (line[1] && line[1][0] && ++num < 7)
			check_textures(line[1], g, &num);
		else if ((line[1] && line[1][0]) || num >= 7)
			g->map = ft_extend_matrix(g->map, line[1]);
		free(line[1]);
	}
	cub_perror(empty_file, g, NULL, !num);
	cub_perror(inv_tex, g, NULL, !g->tex.n || !g->tex.s || !g->tex.e || \
		!g->tex.w || g->tex.floor == -1 || g->tex.ceiling == -1);
	g->height = ft_matrixlen(g->map);
}

void	check_map(t_game *g)
{
	int	j;
	int	i;
	int	w;

	j = -1;
	while (++j < g->height)
	{
		if ((int)ft_strlen(g->map[j]) > g->width)
			g->width = ft_strlen(g->map[j]);
		w = ft_strlen(g->map[j]) - 1;
		i = 0;
		while (ft_isspace(g->map[j][i]) && i <= w)
			i++;
		while (ft_isspace(g->map[j][w]) && w > 0)
			w--;
		if (ft_strncmp(g->map[j], "", 1) == 0)
			cub_perror(inv_map, g, NULL, 1);
		if ((j == 0 || j == g->height - 1) && ft_strlen(g->map[j]) - \
			ft_countchar(g->map[j], ' ') - ft_countchar(g->map[j], '1'))
			cub_perror(inv_wall, g, NULL, 1);
		else if (g->map[j][i] != '1' || g->map[j][w] != '1')
			cub_perror(inv_wall, g, NULL, 1);
		check_elements(g, j);
	}
	cub_perror(inv_map, g, NULL, !j);
}
//problemas: espacios al final de la lines + tabulaciones en vez de espacios + 
