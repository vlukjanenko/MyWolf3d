/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 19:16:55 by majosue           #+#    #+#             */
/*   Updated: 2020/08/29 17:38:49 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_putpixel(t_mlx *mlx, int x, int y, int c)
{
	if (y < mlx->h && y >= 0 && x < mlx->w && x > 0)
		mlx->img.data[y * mlx->w + x] = c;
}

void	ft_fill_rect(t_point start, t_point end, int color, t_mlx *mlx)
{
	int i;
	int j;

	i = start.x;
	j = start.y;
	while (j <= end.y + start.y)
	{
		while (i <= end.x + start.x)
		{
			ft_putpixel(mlx, i, j, color);
			i++;
		}
		i = start.x;
		j++;
	}
}

void	ft_fill_slice(t_point start, t_point end, t_ray ray, t_world *world)
{
	float	k;
	t_point	txtre;
	int		delta;
	int		strt;
	int		color;

	delta = (end.y - start.y > WIN_HEIGHT) ?\
	(end.y - start.y - WIN_HEIGHT) / 2 : 0;
	strt = start.y + delta;
	k = (float)CELL / (end.y - start.y);
	if (ray.texture == 0)
		txtre.x = abs(ray.a.y % CELL);
	if (ray.texture == 1)
		txtre.x = abs(ray.a.x % CELL);
	if (ray.texture == 2)
		txtre.x = abs(ray.a.x % CELL);
	if (ray.texture == 3)
		txtre.x = abs(ray.a.y % CELL);
	while (strt < end.y - delta)
	{
		txtre.y = (strt - start.y) * k;
		color = world->txtre[ray.texture].img_data[world->txtre[ray.texture].w\
		* txtre.y + txtre.x];
		ft_putpixel(world->mlx, start.x, strt++, color);
	}
}

void	ft_draw_slices(t_ray d[WIN_WIDTH], t_world *world)
{
	int		i;
	int		h;
	int		j;
	t_point	start;
	t_point	end;

	i = -1;
	while (++i < WIN_WIDTH)
	{
		h = (CELL / d[i].distance) * world->player->distance_to_win;
		h = h < 0 ? 0 : h;
		j = (WIN_HEIGHT - h) / 2;
		start.x = i;
		end.x = i;
		start.y = j;
		end.y = j + h;
		ft_fill_slice(start, end, d[i], world);
	}
}

void	ft_draw_background(t_mlx *mlx)
{
	t_point start;
	t_point end;

	start.x = 0;
	start.y = 0;
	end.x = WIN_WIDTH;
	end.y = WIN_HEIGHT / 2;
	ft_fill_rect(start, end, 1670625, mlx);
	start.y = end.y;
	end.y = WIN_HEIGHT;
	ft_fill_rect(start, end, 1446932, mlx);
}
