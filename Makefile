# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majosue <majosue@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/13 10:14:46 by majosue           #+#    #+#              #
#    Updated: 2020/08/25 23:14:54 by majosue          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =  wolf3d
HEADER = wolf3d.h
INCLUDES = -I libft/ -I minilibx_macos/
MINILIBX = minilibx_macos/libmlx.a
LIBFT = libft/libft.a
COMPILERC = gcc
FLAGS = -Wall -Wextra -Werror
SOURCES  =  main.c draw.c ft_get_x_cross_d.c ft_get_y_cross_d.c ft_read_map.c \
init.c ft_keypress.c

OBJECTS = $(SOURCES:.c=.o)
.PHONY: clean fclean re all
all: $(NAME)

$(NAME): $(OBJECTS) $(MINILIBX) $(LIBFT)
	$(COMPILERC) -o $(NAME) $(OBJECTS) $(INCLUDES) -L minilibx_macos/ -lmlx -framework OpenGL -framework Appkit -L libft/ -lft
	
$(LIBFT): libft/*.c libft/*.h	
	@make -C libft/

$(MINILIBX): minilibx_macos/*.c minilibx_macos/*.h
	@make -C minilibx_macos/

%.o: %.c $(HEADER)
	$(COMPILERC) $(FLAGS) $(INCLUDES) -o $@ -c $< 

clean:
	@rm -f $(OBJECTS)
	@make -C libft/ clean
	@make -C minilibx_macos/ clean

fclean: clean
	@rm -f $(NAME)
	@make -C libft/ fclean
	@make -C minilibx_macos/ clean

re: fclean all