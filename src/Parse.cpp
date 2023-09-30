/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp           	                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:27:22 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/14 14:27:09 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "colors.hpp"
#include <vector>
#include <string>

namespace
{
//removed the trailing "\r\n"
std::string cleanMsg(Client &user, std::string msg){
	std::string del = "\r\n";
	int end = msg.find_first_of(del, 0); 
	msg = msg.substr(0, end);
	return msg;
}

std::vector<std::string> ircSplitMulti(const std::string& input, const std::string& delimiters) {
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

std::vector<std::string> ircSplit( const std::string& input, const std::string& delimiter){
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

}

void Parse::parseMsg(Client &user, Server *server, std::string msg){
	std::string cmd;
	std::string arguments;
	std::vector<std::string>	args;
	
	std::string clMsg = cleanMsg(user, msg);
	size_t spacePos = clMsg.find(' ');
	if (spacePos != std::string::npos) {
		cmd = clMsg.substr(0, spacePos);
		arguments = clMsg.substr(spacePos + 1);
	}
	else {
		cmd = clMsg.substr(0, spacePos);
		args.push_back(arguments);
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

	Command::parseCmd(user, cmd, args, server);
}


