//********************************************************************//
//* F. PERMADI MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE      *//
//* SUITABILITY OF                                                   *//
//* THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT       *//
//* LIMITED                                                          *//
//* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A      *//
//* PARTICULAR PURPOSE                                               *//
//********************************************************************//

#include "wolf3d.h"
#include <math.h>

void ft_exit(char *str, char *str2)
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

int ft_close(t_mlx *mlx)
{
	mlx_clear_window(mlx->mlx_ptr, mlx->win);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win);
	mlx_destroy_image(mlx->mlx_ptr, mlx->img.img_ptr);
	exit(0);
}

void *ft_grow_array(void *old_mem, int n, size_t size)
{
	void *new_mem;

	if (n <= 0)
		return NULL;
	if (!(new_mem = ft_memalloc(n * size)))
		ft_exit(NULL, NULL);
	if (n == 1)
		return (new_mem);
	ft_memmove(new_mem, old_mem, (n - 1) * size);
	free(old_mem);
	//ft_memdel(old_mem); //не работает
	return (new_mem);
}

int ft_keypress(int keycode, t_world *world)
{
	int new_x;
	int new_y;

	if (keycode == 53)
		ft_close(world->mlx);
	if (keycode == 124)
	{
		world->player->angle = world->player->angle + 2 >= 360 ? world->player->angle + 2 - 360 : world->player->angle + 2;
		ft_draw_world(world);
	}
	if (keycode == 123)
	{
		world->player->angle = world->player->angle - 2 < 0 ? 360 + world->player->angle - 2 : world->player->angle - 2;
		ft_draw_world(world);
	}
	if (keycode == 126)
	{
		world->player->x += cos(rad(world->player->angle)) * world->player->speed;
		world->player->y += sin(rad(world->player->angle)) * world->player->speed;
		ft_draw_world(world);
	}
	if (keycode == 125)
	{
		world->player->x -= cos(rad(world->player->angle)) * world->player->speed;
		world->player->y -= sin(rad(world->player->angle)) * world->player->speed;
		ft_draw_world(world);
		ft_draw_world(world);
	}

	return (EXIT_SUCCESS);
}

/*
**    Mlx init
*/

int ft_mlx_init(t_mlx *mlx, int w, int h, char *title)
{
	mlx->w = w;
	mlx->h = h;
	if (!(mlx->mlx_ptr = mlx_init()) ||
		!(mlx->win = mlx_new_window(mlx->mlx_ptr, w, h, title)) ||
		!(mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, w, h)) ||
		!(mlx->img.data = (int *)mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void ft_fill_rect(t_point start, t_point end, int color, t_mlx *mlx)
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
//*******************************************************************//
//* Draw background image
//*******************************************************************//

void ft_draw_background(t_mlx *mlx)
{
	int c = 25;
	int color;
	t_point start;
	t_point end;

	start.x = 0;
	end.x = WIN_WIDTH;
	end.y = 10;
	for (start.y = 0; start.y < WIN_HEIGHT / 2; start.y += 10)
	{
		color = set_color(c, 125, 225);
		ft_fill_rect(start, end, color, mlx);
		c += 10;
	}
	c = 22;
	end.y = 15;
	for (; start.y < WIN_HEIGHT; start.y += 15)
	{
		color = set_color(c, 20, 20);
		ft_fill_rect(start, end, color, mlx);
		c += 15;
	}
}

int set_color(int r, int g, int b)
{
	t_color color;

	color.argb = 0;
	color.cpnt[0] = b;
	color.cpnt[1] = g;
	color.cpnt[2] = r;
	return (color.argb);
}

void ft_append_line_to_map(int *map, int n, char *line)
{
	int i;

	i = 0;
	while (i < n)
	{
		map[i] = line[i] - '0';
		if (map[i] != 0 && map[i] != 1)
			ft_exit("Wrong map", "");
		i++;
	}
}

void ft_clean_array(char ***array)
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

int ft_array_len(char **array)
{
	int i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void ft_read_textures(char *line, t_world *world)
{
	char **array;
	int i;
	int tmp;

	if (!(array = ft_strsplit(line, ' ')))
		ft_exit(NULL, NULL);
	if (ft_array_len(array) != 4)
		ft_exit("Error: Here hardcoded 4 textures support only.\nAdd 4 textures in the begin of map file", "");
	i = -1;
	while (++i < 4)
	{
		if (!(world->txtre[i].img = mlx_xpm_file_to_image(world->mlx->mlx_ptr, array[i], &(world->txtre[i].w), &(world->txtre[i].h))) ||
			(!(world->txtre[i].img_data = (int *)mlx_get_data_addr(world->txtre[i].img, &tmp, &tmp, &tmp))))
			ft_exit("Error: reading textures", "");
	}
	ft_clean_array(&array);
}

void ft_read_map(int argc, char **argv, t_world *world)
{
	char *line;
	int error;
	int fd;

	world->map_w = -1;
	world->map_h = 0;
	if (argc != 2)
		ft_exit("usage: ./wolf3d file.map", "");
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		ft_exit(NULL, NULL);
	if ((error = get_next_line(fd, &line)) < 0)
		ft_exit(NULL, NULL);
	ft_read_textures(line, world);
	while ((error = get_next_line(fd, &line) > 0))
	{
		if (world->map_w == -1)
			world->map_w = ft_strlen(line);
		else if (world->map_w != ft_strlen(line))
			ft_exit("Wrong map line", "");
		world->map_h++;
		world->map = ft_grow_array(world->map, world->map_h, sizeof(int) * world->map_w);
		ft_append_line_to_map(&world->map[(world->map_h - 1) * world->map_w], world->map_w, line);
	}
}

/* void ft_print_map(int *map, int w, int size)
{
	int i = 0;

	while (i < size)
	{
		if (!(i % w) && i / w)
			printf("\n");
		printf("%d", map[i]);
		
		i++;
	}
} */
double rad(float angle)
{
	return (angle * M_PI / 180);
}

void ft_init_player(t_player *player)
{
	player->angle = 360;
	player->distance_to_win = (WIN_WIDTH / 2) / (tan(rad(FOV / 2)));
	player->speed = 16;
	player->h = 32;
	player->x = 448;
	player->y = 448;
	player->y_center = WIN_HEIGHT / 2;
}

int ft_is_wall(t_world *world, int x, int y)
{
	if (x < 0 || x >= world->map_w || y < 0 || y >= world->map_h)
		return -1;
	return (world->map[world->map_w * y + x]);
}

t_ray ft_minf(t_ray a, t_ray b)
{
	if (b.distance <= a.distance)
		return (b);
	return (a);
}

void ft_get_x_cross0_90(float scaner, t_world *world, t_ray *ray)
{
	t_point a;

	a.y = (world->player->y / CELL) * CELL + CELL;
	a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));

	while (ft_is_wall(world, a.x / CELL, a.y / CELL) == 0)
	{
		a.y += CELL;
		a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));
	}
	if (ft_is_wall(world, a.x / CELL, a.y / CELL) == 1)
		{
			ray->distance = (sqrt(pow(world->player->x - a.x, 2) + pow(world->player->y - a.y, 2)));
			ray->a = a;
			ray->texture = 2;
			return;
		}
	ray->distance = MAXFLOAT;
}

void ft_get_x_cross90_180(float scaner, t_world *world, t_ray *ray)
{
	t_point a;

	a.y = (world->player->y / CELL) * CELL + CELL;
	a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));

	while (ft_is_wall(world, a.x / CELL, a.y / CELL) == 0)
	{
		a.y += CELL;
		a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));
	}
	if (ft_is_wall(world, a.x / CELL, a.y / CELL) == 1)
		{
			ray->distance = (sqrt(pow(world->player->x - a.x, 2) + pow(world->player->y - a.y, 2)));
			ray->a = a;
			ray->texture = 2;
			return;
		}
	ray->distance = MAXFLOAT;
}

void ft_get_x_cross180_270(float scaner, t_world *world, t_ray *ray)
{
	t_point a;

	a.y = (world->player->y / CELL) * CELL -1;
	a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));

	while (ft_is_wall(world, a.x / CELL, a.y / CELL) == 0)
	{
		a.y -= CELL;
		a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));
	}
	if (ft_is_wall(world, a.x / CELL, a.y / CELL) == 1)
		{
			ray->distance = (sqrt(pow(world->player->x - a.x, 2) + pow(world->player->y - a.y, 2)));
			ray->a = a;
			ray->texture = 1;
			return;
		}
	ray->distance = MAXFLOAT;
}

void ft_get_x_cross270_360(float scaner, t_world *world, t_ray *ray)
{
	t_point a;

	a.y = (world->player->y / CELL) * CELL -1;
	a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));

	while (ft_is_wall(world, a.x / CELL, a.y / CELL) == 0)
	{
		a.y -= CELL;
		a.x = roundf(world->player->x + (a.y - world->player->y) / tan(rad(scaner)));
	}
	if (ft_is_wall(world, a.x / CELL, a.y / CELL) == 1)
		{
			ray->distance = (sqrt(pow(world->player->x - a.x, 2) + pow(world->player->y - a.y, 2)));
			ray->a = a;
			ray->texture = 1;
			return;
		}
	ray->distance = MAXFLOAT;
}

void ft_get_x_cross_d(float scaner, t_world *world, t_ray *ray)
{
	if (scaner >= 0 && scaner < 90)
			ft_get_x_cross0_90(scaner, world, ray);
	if (scaner >= 90 && scaner < 180)
			ft_get_x_cross90_180(scaner, world, ray);
	if (scaner >= 180 && scaner < 270)
			ft_get_x_cross180_270(scaner, world, ray);
	if (scaner >= 270 && scaner < 360)
			ft_get_x_cross270_360(scaner, world, ray);
}

void ft_get_y_cross0_90(float scaner, t_world *world, t_ray *ray)
{
	t_point b;

	b.x = world->player->x / CELL * CELL + CELL;
	b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));

	while (ft_is_wall(world, b.x / CELL, b.y / CELL) == 0)
	{
		b.x += CELL;
		b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	}
	if (ft_is_wall(world, b.x / CELL, b.y / CELL) == 1)
		{
		ray->distance = (sqrt(pow(b.x - world->player->x, 2) + pow(b.y - world->player->y, 2)));
		ray->a = b;
		ray->texture = 0;
		return;
		}
	ray->distance = MAXFLOAT;
}

void ft_get_y_cross90_180(float scaner, t_world *world, t_ray *ray)
{
t_point b;

	b.x = world->player->x / CELL * CELL -1;
	b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));

	while (ft_is_wall(world, b.x / CELL, b.y / CELL) == 0)
	{
		b.x -= CELL;
		b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	}
	if (ft_is_wall(world, b.x / CELL, b.y / CELL) == 1)
		{
			ray->distance = (sqrt(pow(b.x - world->player->x, 2) + pow(b.y - world->player->y, 2)));
			ray->a = b;
			ray->texture = 3;
			return;
		}
	ray->distance = MAXFLOAT;
}

void	ft_get_y_cross180_270(float scaner, t_world *world, t_ray *ray)
{
t_point b;

	b.x = world->player->x / CELL * CELL -1;
	b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));

	while (ft_is_wall(world, b.x / CELL, b.y / CELL) == 0)
	{
		b.x -= CELL;
		b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	}
	if (ft_is_wall(world, b.x / CELL, b.y / CELL) == 1)
	{
		ray->distance = (sqrt(pow(b.x - world->player->x, 2) + pow(b.y - world->player->y, 2)));
		ray->a = b;
		ray->texture = 3;
		return;
	}
	ray->distance = MAXFLOAT;
}

void ft_get_y_cross270_360(float scaner, t_world *world, t_ray *ray)
{
t_point b;

	b.x = world->player->x / CELL * CELL + CELL;
	b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));

	while (ft_is_wall(world, b.x / CELL, b.y / CELL) == 0)
	{
		b.x += CELL;
		b.y = world->player->y + (b.x - world->player->x) * tan(rad(scaner));
	}
	if (ft_is_wall(world, b.x / CELL, b.y / CELL) == 1)
		{
			ray->distance = (sqrt(pow(b.x - world->player->x, 2) + pow(b.y - world->player->y, 2)));
			ray->a = b;
			ray->texture = 0;
			return;
		}
		ray->distance = MAXFLOAT;
		ray->a = b;
}

void ft_get_y_cross_d(float scaner, t_world *world, t_ray *ray)
{
	if (scaner >= 0 && scaner < 90)
		ft_get_y_cross0_90(scaner, world, ray);
	if (scaner >= 90 && scaner < 180)
		ft_get_y_cross90_180(scaner, world, ray);
	if (scaner >= 180 && scaner < 270)
		ft_get_y_cross180_270(scaner, world, ray);
	if (scaner >= 270 && scaner < 360)
		ft_get_y_cross270_360(scaner, world, ray);
}

void ft_calculate_distances(t_ray d[WIN_WIDTH], t_world *world)
{
	int i;
	float angle_step;
	float scaner;
	t_ray x_cross_distance;
	t_ray y_cross_distance;

	i = -1;
	angle_step = (float)FOV / WIN_WIDTH;
	scaner = world->player->angle - FOV / 2; // отняи угол
	if (scaner < 0)
		scaner = scaner + 360;
	printf("scaner = %f\n", scaner);

	while (++i < WIN_WIDTH)
	{
		ft_get_x_cross_d(scaner, world, &x_cross_distance);
		ft_get_y_cross_d(scaner, world, &y_cross_distance);
		d[i] = ft_minf(x_cross_distance, y_cross_distance);
		d[i].distance = d[i].distance * cos(rad(world->player->angle - scaner));
		/* printf("distance[%d] = %f\n", i, d[i].distance);
		printf("x_cross_distance = %f\n", x_cross_distance.distance);
		printf("y_cross_distance = %f\n", y_cross_distance.distance); */
		scaner = scaner + angle_step;
		//printf("scaner = %f\n", scaner);
		if (scaner >= 360)
			scaner = 360 - scaner;
	}
	printf("scaner = %f\n", scaner);
}

void ft_fill_slice(t_point start, t_point end, t_ray ray, t_world *world)
{
	float k;
	int h;
	int x;
	int y;
	int strt;
	int color;
	
	strt = start.y;
	h = end.y - start.y;
	k = (float)CELL / h;
	if (ray.texture == 0)
	{
		x = ray.a.y % CELL;
	}
	if (ray.texture == 1)
	{
		x = ray.a.x % CELL;
	}
	if (ray.texture == 2)
	{
		x = ray.a.x % CELL;
	}
	if (ray.texture == 3)
	{
		x = ray.a.y % CELL;
	}
	/* else 
	{
		x = ray.a.y % CELL;
	} */
	while (strt <= end.y)
	{
		y = (strt - start.y) * k;
		color = world->txtre[0].img_data[world->txtre[0].w * y + x];
		ft_putpixel(world->mlx, start.x, strt, color);
		strt++;
	}
}

void ft_draw_slices(t_ray d[WIN_WIDTH], t_world *world)
{
	int i;
	int h;
	int j;
	t_point start;
	t_point end;

	start.c = 0xFFF;
	end.c = 0xFFF;
	start.r = 0;
	end.r = 0;
	i = -1;
	while (++i < WIN_WIDTH)
	{
		h = (CELL / d[i].distance ) * world->player->distance_to_win;
		h = h < 0 ? 0 : h;
		j = (WIN_HEIGHT - h) / 2;
		start.x = i;
		end.x = i;
		start.y = j;
		end.y = j + h;
		ft_fill_slice(start, end, d[i], world);
	//	ft_draw_line(world->mlx, start, end);
	}
}

void ft_draw_world(t_world *world)
{
	t_ray distances[WIN_WIDTH];
	ft_draw_background(world->mlx);
	//-----тут погнали рендер
	printf("player angle = %f\n", world->player->angle);
	ft_calculate_distances(distances, world);
	ft_draw_slices(distances, world);
	//-----тут заончил рендер
	mlx_put_image_to_window(world->mlx->mlx_ptr, world->mlx->win, world->mlx->img.img_ptr, 0, 0);
}

int main(int argc, char **argv)
{
	t_mlx mlx;
	t_world world;
	t_player player;
	void *img;
	int *data;

	ft_init_player(&player);
	world.player = &player;
	if (ft_mlx_init(&mlx, WIN_WIDTH, WIN_HEIGHT, "Wolf3d") == EXIT_FAILURE)
	{
		printf("Error");
		return EXIT_FAILURE;
	}
	world.mlx = &mlx;
	ft_read_map(argc, argv, &world);
	/* for (int i = 0; i <= 360; i++)
	{
		printf("tan(%d) = %f\n", i, tan(rad(i)));
	} */
	ft_draw_world(&world);
	mlx_hook(mlx.win, 2, 1L << 6, ft_keypress, &world);
	mlx_hook(mlx.win, 17, 1l << 17, ft_close, &mlx);
	mlx_loop(mlx.mlx_ptr);
}