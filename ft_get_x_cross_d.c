/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_x_cross_d.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 21:16:40 by majosue           #+#    #+#             */
/*   Updated: 2020/08/25 21:29:30 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		ft_is_wall(t_world *world, int x, int y)
{
	if (x < 0 || x >= world->map_w || y < 0 || y >= world->map_h)
		return (-1);
	return (world->map[world->map_w * y + x]);
}

t_ray	ft_minf(t_ray a, t_ray b)
{
	if (b.distance <= a.distance)
		return (b);
	return (a);
}

void	ft_get_x_cross0_180(float scaner, t_world *world, t_ray *ray)
{
	t_point a;

	a.y = (world->player->y / CELL) * CELL + CELL;
	a.x = world->player->x + (a.y - world->player->y) / tan(rad(scaner));
	while (ft_is_wall(world, a.x / CELL, a.y / CELL) == 0)
	{
		a.y += CELL;
		a.x = world->player->x + (a.y - world->player->y) / tan(rad(scaner));
	}
	if (ft_is_wall(world, a.x / CELL, a.y / CELL) == 1)
	{
		ray->distance = sqrt(pow(world->player->x - a.x, 2) +\
		pow(world->player->y - a.y, 2));
		ray->a = a;
		ray->texture = 2;
		return ;
	}
	ray->distance = MAXFLOAT;
}

void	ft_get_x_cross180_360(float scaner, t_world *world, t_ray *ray)
{
	t_point a;

	a.y = (world->player->y / CELL) * CELL;
	a.x = world->player->x + (a.y - world->player->y) / tan(rad(scaner));
	while (ft_is_wall(world, a.x / CELL, (a.y - 1) / CELL) == 0)
	{
		a.y -= CELL;
		a.x = world->player->x + (a.y - world->player->y) / tan(rad(scaner));
	}
	if (ft_is_wall(world, a.x / CELL, (a.y - 1) / CELL) == 1)
	{
		ray->distance = (sqrt(pow(world->player->x - a.x, 2)\
		+ pow(world->player->y - a.y, 2)));
		ray->a = a;
		ray->texture = 1;
		return ;
	}
	ray->distance = MAXFLOAT;
}

void	ft_get_x_cross_d(float scaner, t_world *world, t_ray *ray)
{
	if (scaner >= 0 && scaner < 180)
		ft_get_x_cross0_180(scaner, world, ray);
	else
		ft_get_x_cross180_360(scaner, world, ray);
}
