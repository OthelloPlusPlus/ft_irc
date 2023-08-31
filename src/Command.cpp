/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:27:22 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/31 15:17:47 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"
#include <vector>
#include <string>

//removed the trailing "\r\n"
void Command::cleanMsg(Client &user){
	std::string del = "\r\n";
	int end = user.getBuff().rfind(del);
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
	else if (cmd == "PASS")
		Command::password(user, cmd, args, server);
	else if (cmd == "NICK")
		Command::nick(user, cmd, args, server->getClientList());
	else if (cmd == "PING")
		Command::ping(user, cmd, args, server);
}

void Command::parseMsg(Client &user, Server *server){
	// Command::cleanMsg(user);
	std::vector<std::string>	cmd;
	cmd = ircSplitMulti(user.getBuff(), "\r\n");
	for (std::vector<std::string>::const_iterator it = cmd.begin(); it != cmd.end(); ++it) {
		const std::string& element = *it;
		size_t spacePos = element.find(' ');
		if (spacePos != std::string::npos) {
			std::string command = element.substr(0, spacePos);
	   	 	std::string params = element.substr(spacePos + 1);
			// parseCmd(user, command, params);
			std::vector<std::string>	args;
			args = ircSplit(params, " ");
			parseCmd(user, command, args, server);
		}
		//change!! no more cmd and args, if I pass only cmd and no space, if condition spacePos != std::string::npos fails and nothing happens
	}
}

/*
PASS :Gatto !!! compare with server pass
USER Emanuela_De_La_Vega * 10.11.1.15 :Marylin vos Savant
NICK Magic
PRIVMSG Bot :a
PING 1692972425
find out what is required from a PONG 

nc 10.11.2.7 6667
*/