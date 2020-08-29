/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_y_cross_d.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 21:24:48 by majosue           #+#    #+#             */
/*   Updated: 2020/08/29 17:05:00 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_exit(char *str, char *str2)
{
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putendl_fd(str2, 2);
		exit(EXIT_FAILURE);
	}
	perror(NULL);
	exit(EXIT_FAILURE);
}

double	rad(float angle)
{
	return (angle * M_PI / 180);
}

void	ft_get_y_cross90_270(float scaner, t_world *world, t_ray *ray)
{
	t_point b;

	b.x = world->player->x / CELL * CELL;
	b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	while (ft_is_wall(world, (b.x - 1), b.y) == 0 && b.y >= 0)
	{
		b.x -= CELL;
		b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	}
	if (ft_is_wall(world, (b.x - 1), b.y) == 1)
	{
		ray->distance = sqrt(pow(world->player->x - b.x, 2)\
		+ pow(world->player->y - b.y, 2));
		ray->a = b;
		ray->texture = 3;
		return ;
	}
	ray->distance = MAXFLOAT;
}

void	ft_get_y_cross270_90(float scaner, t_world *world, t_ray *ray)
{
	t_point b;

	b.x = world->player->x / CELL * CELL + CELL;
	b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	while (ft_is_wall(world, b.x, b.y) == 0 && b.y >= 0)
	{
		b.x += CELL;
		b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	}
	if (ft_is_wall(world, b.x, b.y) == 1)
	{
		ray->distance = sqrt(pow(b.x - world->player->x, 2)\
		+ pow(b.y - world->player->y, 2));
		ray->a = b;
		ray->texture = 0;
		return ;
	}
	ray->distance = MAXFLOAT;
	ray->a = b;
}

void	ft_get_y_cross_d(float scaner, t_world *world, t_ray *ray)
{
	if (scaner >= 90 && scaner < 270)
		ft_get_y_cross90_270(scaner, world, ray);
	else
		ft_get_y_cross270_90(scaner, world, ray);
}
