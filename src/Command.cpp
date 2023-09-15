/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:27:22 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/14 14:27:09 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"
#include <vector>
#include <string>

//removed the trailing "\r\n"
void Command::cleanMsg(Client &user){
	std::string del = "\r\n";
	int end = user.getBuff().find_first_of(del, 0); 
	user.setBuff(user.getBuff().substr(0, end)); 
}

std::vector<std::string> Command::ircSplitMulti(const std::string& input, const std::string& delimiters) {
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
		
	while ((end = input.find_first_of(delimiters, start)) != std::string::npos) {
		if (end != start)
			tokens.push_back(input.substr(start, end - start));
		start = end + 1;
	}
	if (start < input.size())
		tokens.push_back(input.substr(start));
	return tokens;
}

std::vector<std::string> Command::ircSplit( const std::string& input,
								const std::string& delimiter){
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	
	while ((end = input.find(delimiter, start)) != std::string::npos) {
		if (end != start)
			tokens.push_back(input.substr(start, end - start));
		start = end + delimiter.size();
	}
	if (start < input.size())
		tokens.push_back(input.substr(start));
	return tokens;
}

void Command::parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server){
	if (cmd == "USER")
		Command::user(user, cmd, args);
	else if (cmd == "NICK")
		Command::nick(user, cmd, args, server->getClientList());
	else if (cmd == "PASS")
		Command::password(user, cmd, args, server);
	else if (cmd == "PING")
		Command::ping(user, cmd, args, server);
	else if (cmd == "QUIT")
		Command::quit(user, cmd, args, server);
	if (!user.getIsRegistered())
		return;

	if (cmd == "PRIVMSG")
		server->sendPrivMsg(&user, args);
	else if (cmd == "LIST")
		server->sendChannelList(&user);
	else if (cmd == "JOIN")
		server->joinChannel(&user, args[0]);
	else if (cmd == "WHO")
		server->sendWho(&user, args[0]);
	else if (cmd == "WHOIS")
		server->sendWhoIs(&user, args[0]);
	else if (cmd == "PART")
		server->partChannel(&user, args[0]);
	if (!user.getIsOperator())
		return;
		
	if (cmd == "OPER")
		Command::oper(user, cmd, args, server);
}


void Command::parseMsg(Client &user, Server *server){
	std::string cmd;
	std::string arguments;
	std::vector<std::string>	args;
	
	cleanMsg(user);
	size_t spacePos = user.getBuff().find(' ');
	if (spacePos != std::string::npos) {
		cmd = user.getBuff().substr(0, spacePos);
		arguments = user.getBuff().substr(spacePos + 1);
	}
	size_t colPos = arguments.find(':');
	if (colPos != std::string::npos) {
		std::string firstPart = arguments.substr(0, colPos);
		args = ircSplit(firstPart, " ");
		std::string secondPart = arguments.substr(colPos + 1);
        args.push_back(secondPart);
    }
	else {
		size_t space = arguments.find(' ');
		if (space != std::string::npos) {
			args = ircSplit(arguments, " ");
		}
		else {
			args.push_back(arguments);
		}				
	}
	parseCmd(user, cmd, args, server);
}
