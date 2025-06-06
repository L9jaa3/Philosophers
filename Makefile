NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g -fsanitize=thread

SRC = main.c \
      mutex_funs.c \
      philosophers.c \
      protections.c \
      utils.c \
	  helpers.c \
	  monitoring.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
