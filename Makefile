# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/17 10:39:21 by mgraefen          #+#    #+#              #
#    Updated: 2023/04/27 09:33:33 by mgraefen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME 		= 	philo
CC 			= 	cc
CFLAGS 		= 	-Wall -Wextra -Werror -g #-fsanitize=thread
SRC 		= 	philo.c \
				philo_actions.c \
				philo_actions_2.c \
				big_brother.c \
				utils.c
RM 			= 	rm -f
OBJ		= 	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)

clean:
	- @$(RM) $(OBJ)

fclean: clean
	- @$(RM) ${NAME}

re: fclean all

.PHONY: all clean fclean re
