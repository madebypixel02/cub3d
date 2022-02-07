/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:24:16 by aperez-b          #+#    #+#             */
/*   Updated: 2022/02/07 12:50:26 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
	g.map = NULL;
	g.pl.dir = 'M';
	g.mlx_ptr = NULL;
	g.win_img.img = NULL;
	g.mlx_ptr = mlx_init();
	g.tex.north = NULL;
	g.tex.south = NULL;
	g.tex.east = NULL;
	g.tex.west = NULL;
	g.pl.pos.x = -1;
	g.pl.pos.y = -1;
	return (g);
}

int	main(int argc, char **argv)
{
	t_game	g;

	check_file(argc, argv);
	g = cub_init();
	check_map(argv[1], &g);
	game_init(&g);
	//draw_game(&m);
	cub_perror(end, &g, NULL, 1);
	return (0);
}
