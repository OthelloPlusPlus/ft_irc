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

//removed the trailing "\r\n"
void Parse::PrivParsing::cleanMsg(Client &user){
	std::string del = "\r\n";
	int end = user.getBuff().find_first_of(del, 0); 
	user.setBuff(user.getBuff().substr(0, end)); 
}

std::vector<std::string> Parse::PrivParsing::ircSplitMulti(const std::string& input, const std::string& delimiters) {
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

std::vector<std::string> Parse::PrivParsing::ircSplit( const std::string& input,
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


void Parse::parseMsg(Client &user, Server *server){
	std::string cmd;
	std::string arguments;
	std::vector<std::string>	args;
	
	PrivParsing::cleanMsg(user);
	size_t spacePos = user.getBuff().find(' ');
	if (spacePos != std::string::npos) {
		cmd = user.getBuff().substr(0, spacePos);
		arguments = user.getBuff().substr(spacePos + 1);
	}
	size_t colPos = arguments.find(':');
	if (colPos != std::string::npos) {
		std::string firstPart = arguments.substr(0, colPos);
		args = PrivParsing::ircSplit(firstPart, " ");
		std::string secondPart = arguments.substr(colPos + 1);
        args.push_back(secondPart);
    }
	else {
		size_t space = arguments.find(' ');
		if (space != std::string::npos) {
			args = PrivParsing::ircSplit(arguments, " ");
		}
		else {
			args.push_back(arguments);
		}				
	}
	Command::parseCmd(user, cmd, args, server);
}
