/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:24:16 by aperez-b          #+#    #+#             */
/*   Updated: 2022/02/09 16:49:46 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include <mlx.h>

static void	check_file(int argc, char **argv)
{
	int	fd;

	cub_perror(inv_argc, NULL, NULL, argc != 2);
	if (!ft_strncmp(argv[1], "-h", 3) || \
		!ft_strncmp(argv[1], "--help", 7))
		cub_usage(0);
	fd = open(argv[1], O_RDONLY);
	close(fd);
	cub_perror(inv_file, NULL, argv[1], fd < 0);
	if (ft_strrncmp(".cub", argv[1], 4))
		cub_perror(inv_ext, NULL, NULL, 1);
}

static t_game	cub_init(void)
{
	t_game	g;

	g.width = 0;
	g.height = 0;
	g.nframes = 0;
	g.map = NULL;
	g.pl.dir = 0;
	g.mlx_ptr = NULL;
	g.win_ptr = NULL;
	g.win_img.i = NULL;
	g.minimap.i = NULL;
	g.mlx_ptr = mlx_init();
	g.tex.n.i = NULL;
	g.tex.s.i = NULL;
	g.tex.e.i = NULL;
	g.tex.w.i = NULL;
	g.scope.i = NULL;
	g.tex.floor = -1;
	g.tex.ceiling = -1;
	g.pl.x = -1;
	g.pl.y = -1;
	g.pl.speed = 0.06;
	g.mouse_x = 0;
	mlx_load_img(g.mlx_ptr, &g.scope, "textures/scope.xpm");
	return (g);
}

int	main(int argc, char **argv)
{
	t_game	g;

	check_file(argc, argv);
	g = cub_init();
	check_map(argv[1], &g);
	game_init(&g);
	return (0);
}
