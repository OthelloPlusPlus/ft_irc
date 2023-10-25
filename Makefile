# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: ohengelm <ohengelm@student.42.fr>            +#+                      #
#                                                    +#+                       #
#    Created: 2023/10/19 20:26:08 by ohengelm      #+#    #+#                  #
#    Updated: 2023/10/19 20:26:09 by ohengelm      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME =	eavesdrop

CC =	c++
CFLAGS +=	-Wall -Wextra -Werror
INCL_HDR :=	$(shell find . -type f -name '*.hpp' -exec dirname "{}" \; | \
					uniq | sed -e 's/^/-I/g')

SRC =	eavesdrop.cpp

all: $(DIRS) $(NAME)
	@$(CC) $(CFLAGS) $(INCL_HDR) $(SRC) -o $(NAME)
	@printf	"Running eavesdrop:\n"
	@./eavesdrop

clean:
	@rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
