FILES :=	pollserver\
			pollclient\
			gptserver\
			gptclient\
			eavesdrop

all: $(FILES)

%: %.cpp
	c++ $< -o $@

clean:
	rm -f $(FILES)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
