/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/25 01:19:41 by lmarques          #+#    #+#             */
/*   Updated: 2020/08/25 22:31:57 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include <fcntl.h>
# include <stdio.h>

# define WIN_WIDTH 1280
# define WIN_HEIGHT 1024
# define CELL 256
# define FOV 60

typedef struct	s_point
{
	int		x;
	int		y;
}				t_point;

typedef struct	s_img
{
	void	*img_ptr;
	int		*data;
	int		size_l;
	int		bpp;
	int		endian;
}				t_img;

typedef struct	s_mlx
{
	void		*mlx_ptr;
	void		*win;
	int			w;
	int			h;
	t_img		img;
}				t_mlx;

typedef	struct	s_player
{
	int		x;
	int		y;
	float	angle;
	int		h;
	int		speed;
	int		distance_to_win;
}				t_player;

typedef struct	s_txtre
{
	void	*img;
	int		*img_data;
	int		w;
	int		h;
}				t_txtre;

typedef	struct	s_ray
{
	float	distance;
	t_point	a;
	int		texture;
}				t_ray;

typedef	struct	s_world
{
	t_txtre		txtre[4];
	t_mlx		*mlx;
	t_player	*player;
	int			*map;
	int			map_w;
	int			map_h;
}				t_world;

void			ft_putpixel(t_mlx *mlx, int x, int y, int c);
void			ft_draw_background(t_mlx *mlx);
void			ft_draw_slices(t_ray d[WIN_WIDTH], t_world *world);
void			ft_fill_slice(t_point start, t_point end, t_ray ray,\
t_world *world);
void			ft_draw_world(t_world *world);
int				ft_is_wall(t_world *world, int x, int y);
t_ray			ft_minf(t_ray a, t_ray b);
void			ft_get_x_cross_d(float scaner, t_world *world, t_ray *ray);
void			ft_get_y_cross_d(float scaner, t_world *world, t_ray *ray);
double			rad(float angle);
void			ft_exit(char *str, char *str2);
void			ft_read_map(int argc, char **argv, t_world *world);
void			ft_clean_array(char ***array);
int				ft_close(t_mlx *mlx);
void			ft_put_player(t_world *world);
void			ft_init_world(t_world *world);
void			ft_mlx_init(t_mlx *mlx, int w, int h, char *title);
int				ft_keypress(int keycode, t_world *world);

#endif
