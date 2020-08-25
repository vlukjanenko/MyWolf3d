/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/25 01:19:41 by lmarques          #+#    #+#             */
/*   Updated: 2020/08/25 16:24:33 by majosue          ###   ########.fr       */
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

# define WIN_WIDTH 640
# define WIN_HEIGHT 480
# define CELL 64

# define FOV 60

typedef struct	s_point
{
	int		x;
	int		y;
	int		r;
	int		c;
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
    int         w;
    int         h;
	t_img		img;
}				t_mlx;

typedef	union	u_color
{
	unsigned int	argb;
	unsigned char	cpnt[4];
}				t_color;

typedef	struct	s_player
{
	int x;
	int y;
	float angle;
	int h;
	int speed;
	int distance_to_win;
	int y_center;
}				t_player;

typedef struct s_txtre
{
	void	*img;
	int		*img_data;
	int		w;
	int		h;
}				t_txtre;

typedef	struct s_ray
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

void	ft_putpixel(t_mlx *mlx, int x, int y, int c);
void	ft_toradius(t_point *start, t_point *end);
int		set_color(int r, int g, int b);
int		ft_draw_line(t_mlx *mlx, t_point start, t_point end);
void	ft_draw_world(t_world *world);
int 	ft_is_wall(t_world *world, int x, int y);
double rad(float angle);



/* char			**ft_readmap(int fd, int *width, int *height, int *number);
int				ft_draw_line(t_mlx *mlx, t_point p1, t_point p2);
int				ft_atoi_base(const char *nbr, unsigned int base);
int				ft_rotx(int key, t_mlx *mlx, double a);
int				ft_roty(int key, t_mlx *mlx, double a);
int				ft_draw(t_mlx *mlx);
void			ft_events(t_mlx *mlx);
int				ft_close(t_mlx *mlx);
void			ft_win_to_log(t_mlx *mlx);
int				ft_set_colour(t_point *point, int zmax);
int				ft_getcolor(t_point p1, t_point end, t_point current);
void			ft_reset(int key, t_mlx *mlx);
int				ft_find_zmax(char **a);
int				ft_find_d(t_mlx *mlx, int zmax);
void			ft_put_help(t_mlx *mlx); */

#endif
