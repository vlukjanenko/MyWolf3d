/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 19:16:55 by majosue           #+#    #+#             */
/*   Updated: 2020/08/19 14:08:32 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include <stdio.h>
#include <math.h>

void	ft_swap(t_point *p1, t_point *p2)
{
	t_point tmp;

	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void	ft_putpixel(t_mlx *mlx, int x, int y, int c)
{
	if (y < mlx->h && y >= 0 && x < mlx->w && x > 0)
		mlx->img.data[y * mlx->w + x] = c;
}

void	ft_xstep(t_mlx *mlx, t_point start, t_point end)
{
	int deltax;
	int deltay;
	int error;
	t_point current;
	int diry;

	diry = (end.y - start.y >= 0) ? 1 : -1; 
	current = start;
	deltax = abs(end.x - start.x);
	deltay = abs(end.y - start.y);
	error = 0;

	while (current.x <= end.x)
	{
		ft_putpixel(mlx, current.x, current.y, current.c);
		error = error + deltay + 1;
		if (error >= (deltax + 1))
		{
			current.y += diry;
			error = error - (deltax + 1);
		}
		current.x++;
	}
}

void	ft_ystep(t_mlx *mlx, t_point start, t_point end)
{
	int deltax;
	int deltay;
	int error;
	t_point current;
	int dirx;

	dirx = (end.x - start.x >= 0) ? 1 : -1;
	current = start;
	deltax = abs(end.x - start.x);
	deltay = abs(end.y - start.y);
	error = 0;
	while (current.y <= end.y)
	{
		ft_putpixel(mlx, current.x, current.y, current.c);
		error = error + deltax + 1;
		if (error >= (deltay + 1))
		{
			current.x += dirx;
			error = error - (deltay + 1);
		}
        current.y++;
	}
}

int		ft_draw_line(t_mlx *mlx, t_point start, t_point end)
{
	
	if (abs(end.x - start.x) >= abs(end.y - start.y))
	{
		end.x < start.x ? ft_swap(&end, &start) : end.x;
		ft_toradius(&start, &end);
		ft_xstep(mlx, start, end);
	}
	else
	{
		end.y < start.y ? ft_swap(&end, &start) : end.y;
		ft_toradius(&start, &end);
		ft_ystep(mlx, start, end);
	}
	return (1);
}

void ft_draw_circle(t_mlx *mlx, t_point p)
{
   int x = 0;
   int y = p.r;
   int delta = 1 - 2 * p.r;
   int error = 0;

   while (y >= 0)
   {
	   ft_putpixel(mlx, p.x + x, p.y + y, p.c);
	   ft_putpixel(mlx, p.x + x, p.y - y, p.c);
       ft_putpixel(mlx, p.x - x, p.y + y, p.c);
	   ft_putpixel(mlx, p.x - x, p.y - y, p.c);	   
       error = 2 * (delta + y) - 1;
       if ((delta < 0) && (error <= 0))
       {
           delta += 2 * ++x + 1;
           continue;
       }
       if ((delta > 0) && (error > 0))
       {
           delta -= 2 * --y + 1;
           continue;
       }
       delta += 2 * (++x - --y);
   }
}

/* int ft_mlx_init(t_mlx *mlx, int w, int h, char *title)
{
    mlx->w = w;
    mlx->h = h;
    if (!(mlx->mlx_ptr = mlx_init()) || 
    !(mlx->win = mlx_new_window(mlx->mlx_ptr, w, h, title)) ||
    !(mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, w, h)) || 
    !(mlx->img.data = (int*)mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian)))
        return (EXIT_FAILURE);
    return(EXIT_SUCCESS);
} */

void ft_toradius(t_point *start, t_point *end) 
{
	double k;
	t_point p1;
	t_point p2;
 
	if (end->x == start->x)
    {
		start->y += start->r;
		end->y -= end->r;
		return;
    }
	k = ((double)end->y - start->y) / ((double)end->x - start->x);
	p1.x = (end->x > start->x) ? sqrt(pow(start->r, 2) / (1.0 + pow(k, 2))) : -sqrt(pow(start->r, 2) / (1.0 + pow(k, 2)));
	p1.y = (end->y > start->y) ? k * p1.x : -fabs(k * p1.x);
	p2.x = (end->x < start->x) ? sqrt(pow(end->r, 2) / (1.0 + pow(k, 2))) : -sqrt(pow(end->r, 2) / (1.0 + pow(k, 2)));
	p2.y = (end->y < start->y) ? k * p2.x : -fabs(k * p2.x);  
	start->y += p1.y;
	start->x += p1.x;
	end->y += p2.y;
	end->x += p2.x;
}