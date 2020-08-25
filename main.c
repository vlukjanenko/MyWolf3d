/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 20:59:45 by majosue           #+#    #+#             */
/*   Updated: 2020/08/25 22:16:24 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_calculate_distances(t_ray d[WIN_WIDTH], t_world *world)
{
	int		i;
	float	angle_step;
	float	scaner;
	t_ray	x_cross_distance;
	t_ray	y_cross_distance;

	i = -1;
	angle_step = (float)FOV / WIN_WIDTH;
	scaner = world->player->angle - FOV / 2;
	if (scaner < 0)
		scaner = scaner + 360;
	while (++i < WIN_WIDTH)
	{
		ft_get_x_cross_d(scaner, world, &x_cross_distance);
		ft_get_y_cross_d(scaner, world, &y_cross_distance);
		d[i] = ft_minf(x_cross_distance, y_cross_distance);
		d[i].distance = d[i].distance * cos(rad(world->player->angle - scaner));
		scaner = scaner + angle_step;
		if (scaner >= 360)
			scaner = 360 - scaner;
	}
}

void	ft_draw_world(t_world *world)
{
	t_ray distances[WIN_WIDTH];

	ft_draw_background(world->mlx);
	ft_calculate_distances(distances, world);
	ft_draw_slices(distances, world);
	mlx_put_image_to_window(world->mlx->mlx_ptr, world->mlx->win,
							world->mlx->img.img_ptr, 0, 0);
}

int		main(int argc, char **argv)
{
	t_mlx		mlx;
	t_world		world;
	t_player	player;

	world.player = &player;
	ft_init_world(&world);
	ft_mlx_init(&mlx, WIN_WIDTH, WIN_HEIGHT, "Wolf3d");
	world.mlx = &mlx;
	ft_read_map(argc, argv, &world);
	ft_put_player(&world);
	ft_draw_world(&world);
	mlx_hook(mlx.win, 2, 1L << 6, ft_keypress, &world);
	mlx_hook(mlx.win, 17, 1l << 17, ft_close, &mlx);
	mlx_loop(mlx.mlx_ptr);
}
