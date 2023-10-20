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

NAME =	ircserv

CC =	c++
CFLAGS +=	-Wall -Wextra -Werror
# ifeq ($(shell uname), Darwin)
CFLAGS +=	-std=c++11
# else
# 	CFLAGS +=	-std=c++98
# endif
DEPFLAGS +=	-MMD -MF $(DEP_DIR)$*.d

INCL_HDR :=	$(shell find . -type f -name '*.hpp' -exec dirname "{}" \; | \
					uniq | sed -e 's/^/-I/g')

SRC =	$(SRC_MAIN:%.cpp=	$(SRC_DIR)%.cpp)\
		$(SRC_ACLASS:%.cpp=	$(SRC_DIR)%.cpp)\
		$(SRC_CLASS:%.cpp=	$(SRC_DIR)%.cpp)\
		$(SRC_FUNC:%.cpp=	$(SRC_DIR)%.cpp)\
		$(SRC_NMSPC:%.cpp=	$(SRC_DIR)%.cpp)

SRC_DIR =	src/
SRC_MAIN =		main.cpp
SRC_FUNC =		setEnv.cpp verboseCheck.cpp
SRC_CLASS =		Server.cpp Channel.cpp
SRC_ACLASS =	AClient.cpp Client.cpp ServerBot.cpp BotTicTacToe.cpp RockBot.cpp
SRC_NMSPC =		Parse.cpp Command.cpp

DIRS =	$(OBJ_DIR) $(DEP_DIR)
OBJ_DIR =	obj/
OBJ =	$(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)
DEP_DIR =	dep/
DEP =	$(SRC:$(SRC_DIR)%.cpp=$(DEP_DIR)%.d)

all: $(DIRS) $(NAME)

$(DIRS):
	@mkdir $(DIRS)

$(NAME): $(OBJ)
	@printf	"Linking object files together in binary: %s\n"	$@
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@printf	"Compiling object file: %s\n"	$@
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(INCL_HDR) -c $< -o $@

-include $(DEP)

subject: all
	./$(NAME) 6667 password

eaves:#remove (also in fclean)
	@$(CC) ./functiontesting/poll/eavesdrop.cpp -o eavesdrop
	@printf	"Running eavesdrop:\n"
	@./eavesdrop

clean:
	@rm -f $(OBJ)
	@rm -f $(DEP)

fclean: clean
	@rm -f $(NAME)
	@rm -f eavesdrop
	@rm -f ._*
	@rmdir $(DIRS) 2> /dev/null || true

re: fclean all

.PHONY: all clean fclean re subject
