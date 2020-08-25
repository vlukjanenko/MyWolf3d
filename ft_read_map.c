/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 21:35:32 by majosue           #+#    #+#             */
/*   Updated: 2020/08/25 23:15:53 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		ft_array_len(char **array)
{
	int i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	*ft_grow_array(void *old_mem, int n, size_t size)
{
	void *new_mem;

	if (n <= 0)
		return (NULL);
	if (!(new_mem = ft_memalloc(n * size)))
		ft_exit(NULL, NULL);
	if (n == 1)
		return (new_mem);
	ft_memmove(new_mem, old_mem, (n - 1) * size);
	free(old_mem);
	return (new_mem);
}

void	ft_append_line_to_map(int *map, int n, char *line)
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

void	ft_read_textures(int fd, t_world *world)
{
	char	**array;
	int		i;
	int		tmp;
	char	*line;

	if (get_next_line(fd, &line) < 0)
		ft_exit(NULL, NULL);
	if (!(array = ft_strsplit(line, ' ')))
		ft_exit(NULL, NULL);
	if (ft_array_len(array) != 4)
		ft_exit("Error: Here hardcoded 4 textures support only.\n\
		Add 4 textures in the begin of map file", "");
	i = -1;
	while (++i < 4)
	{
		if (!(world->txtre[i].img = mlx_xpm_file_to_image(world->mlx->mlx_ptr,\
		array[i], &(world->txtre[i].w), &(world->txtre[i].h))) ||
			(!(world->txtre[i].img_data =\
			(int *)mlx_get_data_addr(world->txtre[i].img, &tmp, &tmp, &tmp))))
			ft_exit("Error: reading textures", "");
	}
	ft_strdel(&line);
	ft_clean_array(&array);
}

void	ft_read_map(int argc, char **argv, t_world *world)
{
	char	*line;
	int		error;
	int		fd;

	if (argc != 2)
		ft_exit("usage: ./wolf3d file.map", "");
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		ft_exit(NULL, NULL);
	ft_read_textures(fd, world);
	while ((error = get_next_line(fd, &line) > 0))
	{
		if (world->map_w == -1)
			world->map_w = ft_strlen(line);
		else if (world->map_w != (int)ft_strlen(line))
			ft_exit("Wrong map line", "");
		world->map_h++;
		world->map = ft_grow_array(world->map, world->map_h,\
		sizeof(int) * world->map_w);
		ft_append_line_to_map(&world->map[(world->map_h - 1)\
		* world->map_w], world->map_w, line);
		ft_strdel(&line);
	}
	if (error == -1)
		ft_exit(NULL, NULL);
	close(fd);
}
