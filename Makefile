CC		= gcc

NAME		= my_objdump

SRCS_DIR 	= srcs

CFLAGS		+= -W -Wall -Wextra
CPPFLAGS	+= -Iinclude

SRCS		= $(SRCS_DIR)/main.c			\
		  $(SRCS_DIR)/utils.c		\
		  $(SRCS_DIR)/elf_header.c		\
		  $(SRCS_DIR)/getters.c		\
		  $(SRCS_DIR)/printer.c

OBJS		= $(SRCS:.c=.o)

CP		= cp
RM		= rm -f
PRINTF		= @printf

$(NAME):	$(OBJS)
			$(CC) $(OBJS) -o $(NAME)
			$(PRINTF) '\033[1;32m> Compiled\033[0m\n'

all:			$(NAME)

clean:
			$(RM) $(OBJS)
			$(PRINTF) '\033[1;35m> Directory cleaned\033[0m\n'

fclean:			clean
			$(RM) $(NAME)
			$(PRINTF) '\033[1;35m> Executable removed\033[0m\n'

re:			fclean all

.PHONY:			all clean fclean re nm objdump
