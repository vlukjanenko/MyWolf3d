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

int		ft_close(t_mlx *mlx)
{
	mlx_clear_window(mlx->mlx_ptr, mlx->win);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win);
	mlx_destroy_image(mlx->mlx_ptr, mlx->img.img_ptr);
	exit(0);
}

void	*ft_grow_array(void *old_mem, int n, size_t size)
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

int		ft_keypress(int keycode, t_world *world)
{
	if (keycode == 53)
		ft_close(world->mlx);
	if (keycode == 123)
		{
			world->player->angle = world->player->angle == 360 ? 0 : world->player->angle + 1;
			ft_draw_world(world);
		}
	if (keycode == 124)
		{
			world->player->angle =  world->player->angle == 0 ? 360 : world->player->angle - 1;
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
	while(i < n)
	{
		map[i] = line[i] - '0';
		if (map[i] != 0 && map[i] != 1)
			ft_exit("Wrong map", "");
		i++;
	}
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

int		ft_array_len(char **array)
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
		if (!(world->txtre[i].img = mlx_xpm_file_to_image(world->mlx->mlx_ptr, array[i], &(world->txtre[i].w), &(world->txtre[i].h))) ||\
		(!(world->txtre[i].img_data = (int *)mlx_get_data_addr(world->txtre[i].img, &tmp, &tmp, &tmp))))
			ft_exit("Error: reading textures", "");
	}
	ft_clean_array(&array);
}

void 	ft_read_map(int argc, char **argv, t_world *world)
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
	return(angle * M_PI / 180);
}

void ft_init_player(t_player *player)
{
	player->angle = 60;
	player->distance_to_win = (WIN_WIDTH / 2) / (tan(rad(player->angle / 2)));
	player->speed = 16;
	player->h = 32;
	player->x = 300;
	player->y = 524;
	player->y_center = WIN_HEIGHT / 2;
}



int ft_is_wall(t_world *world, int x, int y)
{
	if (x < 0 || x >= world->map_w || y < 0 || y >= world->map_h)
		return -1;
	return (world->map[world->map_w * y + x]);
}

float ft_minf(float a, float b)
{
	if (b < a)
		return (b);
	return (a);
}

float	ft_get_x_cross_d(float scaner, t_world *world)
{
	t_point a;
	t_point delta;

	if (scaner == 0 || scaner == 180)
		return (MAXFLOAT);
	if (scaner > 0 && scaner < 180)
	{
		a.y = (world->player->y / CELL) * CELL + CELL;
		delta.y = CELL;
	}
	else
	{
		a.y = (world->player->y / CELL) * CELL - 1;	
		delta.y = -CELL;
	} 
		a.x = world->player->x + (world->player->y - a.y) / tan(rad(scaner));
	if (scaner > 180 && scaner < 270)	
		delta.x = -CELL / tan(rad(scaner));
	else
	{
		delta.x = CELL / tan(rad(scaner));
	}
	
	
	while (ft_is_wall(world, a.x / CELL, a.y / CELL) == 0)
	{
		a.y += delta.y;
		a.x += delta.x;
	}
	if (ft_is_wall(world, a.x / CELL, a.y / CELL) == 1)
		return(sqrt(pow(world->player->x - a.x, 2) + pow(world->player->y - a.y, 2)));
	return (MAXFLOAT);
}

float	ft_get_y_cross_d(float scaner, t_world *world)
{
	t_point b;
	t_point delta;
	if (scaner == 90 || scaner == 270)
		return(MAXFLOAT);
	if (scaner > 90 && scaner < 270)
	{
		b.x = world->player->x / CELL * CELL - 1;
		delta.x = -CELL;
	}
	else
	{
		b.x = world->player->x / CELL * 64 + 64;
		delta.x = CELL;
	}
	delta.y = (CELL + 1) * tan(rad(scaner));
	b.y = world->player->y + (world->player->x - b.x) * tan(rad(scaner));
	
	while (ft_is_wall(world, b.x / CELL, b.y / CELL) == 0)
	{
		b.y += delta.y;
		b.x += delta.x;
	}
	if (ft_is_wall(world, b.x / CELL, b.y / CELL) == 1)
		return(sqrt(pow(world->player->x - b.x, 2) + pow(world->player->y - b.y, 2)));
	return (MAXFLOAT);
}

void ft_calculate_distances(float d[WIN_WIDTH], t_world *world)
{
	int i;
	float angle_step;
	float scaner;
	float x_cross_distance;
	float y_cross_distance;
	
	i = -1;	
	angle_step = (float)world->player->angle / WIN_WIDTH;
	scaner = world->player->angle - FOV / 2; // отняи угол
	if (scaner < 0)
		scaner = scaner + 360;
	while (++i < WIN_WIDTH)
	{
		x_cross_distance = ft_get_x_cross_d(scaner, world);
		y_cross_distance = ft_get_y_cross_d(scaner, world);
		d[i] = ft_minf(x_cross_distance, y_cross_distance);
		d[i] = d[i] * cos(rad(scaner - world->player->angle));
		//printf("distance[%d] = %f\n", i,d[i]);
		//printf("x_cross_distance = %f\n", x_cross_distance);
		//printf("y_cross_distance = %f\n", y_cross_distance);
		scaner = scaner + angle_step;
	}
}

void ft_draw_slices(float distances[WIN_WIDTH], t_world *world)
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
		h = (CELL / distances[i]) * world->player->distance_to_win;
		j = (WIN_HEIGHT - h) / 2;
		start.x = i;
		end.x = i;
		start.y = j;
		end.y = j + h;
		ft_draw_line(world->mlx, start, end);
	}
}

void ft_draw_world(t_world *world)
{
	float distances[WIN_WIDTH];
	ft_draw_background(world->mlx);
	//-----тут погнали рендер
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
    if (ft_mlx_init(&mlx, WIN_WIDTH, WIN_HEIGHT, "test") == EXIT_FAILURE)
    {
        printf("Error");
        return EXIT_FAILURE;
    }
    world.mlx = &mlx;
	ft_read_map(argc, argv, &world);
	ft_draw_world(&world);
    mlx_hook(mlx.win, 2, 1L << 6, ft_keypress, &world);
	mlx_hook(mlx.win, 17, 1l << 17, ft_close, &mlx);
    mlx_loop(mlx.mlx_ptr);
}