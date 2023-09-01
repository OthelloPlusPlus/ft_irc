NAME =	ircserv

CC =	c++
# CFLAGS +=	-Wall -Wextra -Werror
CFLAGS +=	-std=c++98
CFLAGS +=	-Wno-unused-variable
DEPFLAGS +=	-MMD -MF $(DEP_DIR)$*.d

INCL_HDR :=	$(shell find . -type f -name '*.hpp' -exec dirname "{}" \; | \
					uniq | sed -e 's/^/-I/g')

SRC =	$(SRC_MAIN:%.cpp=	$(SRC_DIR)%.cpp)\
		$(SRC_CLASS:%.cpp=	$(SRC_DIR)%.cpp)\
		$(SRC_FUNC:%.cpp=	$(SRC_DIR)%.cpp)\
		$(SRC_NMSPC:%.cpp=	$(SRC_DIR)%.cpp)

SRC_DIR =	src/
SRC_MAIN =	main.cpp
SRC_CLASS =	Server.cpp Client.cpp Channel.cpp
SRC_FUNC =	
SRC_NMSPC =	Command.cpp		\
			CommandNick.cpp CommandPass.cpp CommandUser.cpp CommandPing.cpp \
			CommandQuit.cpp CommandOper.cpp

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
