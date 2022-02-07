/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 16:43:33 by mbueno-g          #+#    #+#             */
/*   Updated: 2022/02/07 22:50:17 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	create_trgb(t_color c)
{
	return (c.t << 24 | c.r << 16 | c.g << 8 | c.b);
}

void	get_cf_color(char **dir, t_game *g)
{
	char	**fc;
	int		f;
	int		c[3];
	t_color	aux;

	aux.t = 0;
	f = !ft_strncmp(dir[0], "F", 2);
	fc = ft_split(dir[1], ',');
	cub_perror(inv_color, g, dir[0], !fc);
	c[0] = cub_atoi(fc[0], &aux.r);
	c[1] = cub_atoi(fc[1], &aux.g);
	c[2] = cub_atoi(fc[2], &aux.b);
	ft_free_matrix(&fc);
	if (c[0] || c[1] || c[2])
		cub_perror(inv_color, g, dir[1], 1);
	if (f)
		g->tex.floor = create_trgb(aux);
	else
		g->tex.ceiling = create_trgb(aux);
}
