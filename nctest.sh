#! /bin/bash

IRC_SERVER=$(hostname -I | awk '{print $1}')
PORT=6667
PASSWORD="password"
USER1="OthelloNC"
USER2="OtheNetCat"
NICK="OthNC"

{
	echo "PASS $PASSWORD"
	echo "USER $USER1 * $IRC_SERVER :$USER2"
	echo "NICK $NICK"
	sleep 0.1
	while [ true ]; do
		read -p "nc (or quit): " COMMAND;
		if [ "$COMMAND" == "quit" ]; then
			echo "QUIT :leaving...";
			break ;
		else
			echo "$COMMAND";
		fi
		sleep 0.2
	done
} | nc -v $IRC_SERVER $PORT
