/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:27:22 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/31 16:54:36 by emlicame         ###   ########.fr       */
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
	else if (cmd == "QUIT")
		Command::ping(user, cmd, args, server);
	else if (cmd == "LIST"){
		server->sendChannelList(&user);
	}

}

void Command::parseMsg(Client &user, Server *server){
	std::vector<std::string>	cmd;
	cmd = ircSplitMulti(user.getBuff(), "\r\n");
	for (std::vector<std::string>::const_iterator it = cmd.begin(); it != cmd.end(); ++it) {
		const std::string& element = *it;
		size_t spacePos = element.find(' ');
		if (spacePos != std::string::npos) {
			std::string command = element.substr(0, spacePos);
	   	 	std::string params = element.substr(spacePos + 1);
			std::cout << "with space " << command << " and args " << params << std::endl;
			std::vector<std::string>	args;
			args = ircSplit(params, " ");
			parseCmd(user, command, args, server);
		}
		else {
			std::string command = element;
			std::vector<std::string>	args;
			parseCmd(user, command, args, server);
		}			
	}
}
