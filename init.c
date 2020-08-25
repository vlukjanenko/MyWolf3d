/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 21:51:10 by majosue           #+#    #+#             */
/*   Updated: 2020/08/25 22:20:20 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_mlx_init(t_mlx *mlx, int w, int h, char *title)
{
	mlx->w = w;
	mlx->h = h;
	if (!(mlx->mlx_ptr = mlx_init()) ||
		!(mlx->win = mlx_new_window(mlx->mlx_ptr, w, h, title)) ||
		!(mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, w, h)) ||
		!(mlx->img.data = (int *)mlx_get_data_addr(mlx->img.img_ptr,\
		&mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian)))
		ft_exit(NULL, NULL);
}

void	ft_init_world(t_world *world)
{
	world->player->angle = 360;
	world->player->distance_to_win = (WIN_WIDTH / 2) / (tan(rad(FOV / 2)));
	world->player->speed = 16;
	world->player->h = 32;
	world->player->x = -1;
	world->player->y = -1;
	world->map_w = -1;
	world->map_h = 0;
}

void	ft_put_player(t_world *world)
{
	int i;

	i = -1;
	while (++i < world->map_h * world->map_w)
	{
		if (world->map[i] == 0)
		{
			world->player->y = (i / world->map_w) * CELL + CELL / 2;
			world->player->x = (i % world->map_w) * CELL + CELL / 2;
			break ;
		}
	}
	if (world->player->x < 0)
		ft_exit("No space for player on map", "");
}

int		ft_close(t_mlx *mlx)
{
	mlx_clear_window(mlx->mlx_ptr, mlx->win);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win);
	mlx_destroy_image(mlx->mlx_ptr, mlx->img.img_ptr);
	exit(0);
}

void	ft_clean_array(char ***array)
{
	char **tmp;

	if (*array)
	{
		tmp = *array;
		while (**array)
		{
			free(**array);
			(*array)++;
		}
		free(tmp);
	}
}
