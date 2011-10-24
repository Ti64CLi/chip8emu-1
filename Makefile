CC	=	gcc
RM	=	rm -f
STRIP	=	strip
NAME	=	chip8emu
SRC	+=	main.c		\
		cpu_chip8.c	\
		graphic_view.c
OBJ	=	$(SRC:.c=.o)
CFLAGS	+=	-W -Werror -ansi -pedantic -O2
AFLAGS	+=	-lSDL

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(AFLAGS)
	$(STRIP) $(NAME)

all:		$(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) *~

fclean:		clean
	$(RM) $(NAME)

re:		clean	$(NAME)
