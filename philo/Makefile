
NAME = philo
SOURCES = main.c actions.c ft_atoi.c check_time.c \
			unlock.c start.c parsing.c
OFILES = $(SOURCES:.c=.o)
CC = cc
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OFILES)
	$(CC) $(FLAGS) $(OFILES) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -I. -o $@

clean:
	rm -f $(OFILES)
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY: all, clean, fclean, re