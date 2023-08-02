all:
	c++ pollserver.cpp -o pollserver
	c++ pollclient.cpp -o pollclient

clean:
	rm -f pollclient
	rm -f pollserver

fclean: clean

re: fclean all

.PHONY: all clean fclean re
