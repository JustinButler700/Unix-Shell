## Apple clang version 12.0.5 (clang-1205.0.22.11)
## Target: x86_64-apple-darwin20.6.0
## Thread model: posix

CC		=	gcc

RM		=	rm -f

NAME	=	shell

SRC		=	main.c	\

CFLAGS	=	-Wall -Wextra -Werror -g3

OBJ		=	$(SRC:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) -o $(NAME) $(CFLAGS) $(OBJ)

clean:
			$(RM) $(OBJ)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re