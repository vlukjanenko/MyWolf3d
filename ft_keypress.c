/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_keypress.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 22:09:49 by majosue           #+#    #+#             */
/*   Updated: 2020/08/25 22:12:27 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_move_forward(t_world *world)
{
	int new_x;
	int new_y;

	new_x = cos(rad(world->player->angle)) * world->player->speed;
	new_y = sin(rad(world->player->angle)) * world->player->speed;
	if (!ft_is_wall(world, (world->player->x + new_x + 5) / CELL,\
	(world->player->y + new_y + 5) / CELL))
	{
		world->player->x += new_x;
		world->player->y += new_y;
	}
}

void	ft_move_back(t_world *world)
{
	int new_x;
	int new_y;

	new_x = cos(rad(world->player->angle)) * world->player->speed;
	new_y = sin(rad(world->player->angle)) * world->player->speed;
	if (!ft_is_wall(world, (world->player->x - new_x - 5) / CELL,\
	(world->player->y - new_y - 5) / CELL))
	{
		world->player->x -= new_x;
		world->player->y -= new_y;
	}
}

int		ft_keypress(int keycode, t_world *world)
{
	if (keycode == 53)
		ft_close(world->mlx);
	if (keycode == 124)
		world->player->angle = world->player->angle + 2 >= 360 ?\
		world->player->angle + 2 - 360 : world->player->angle + 2;
	if (keycode == 123)
		world->player->angle = world->player->angle - 2 < 0 ?\
		360 + world->player->angle - 2 : world->player->angle - 2;
	if (keycode == 126)
		ft_move_forward(world);
	if (keycode == 125)
		ft_move_back(world);
	ft_draw_world(world);
	return (EXIT_SUCCESS);
}
