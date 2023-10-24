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

#include <string>
// std::string
#include <vector>
// std::vector
#include <tuple>
// for std::tuple

#include <algorithm>
// std::transform

namespace {

//removed the trailing "\r\n"
std::string cleanMsg(std::string msg) {

	std::string del = "\r\n";
	int end = msg.find_first_of(del, 0); 
	msg = msg.substr(0, end);
	return msg;
}

std::vector<std::string> ircSplit( const std::string& input, const std::string& delimiter) {

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

std::tuple<AClient &, std::string, std::vector<std::string> > Parse::parseMsg(AClient &user, std::string msg) {

	std::string cmd;
	std::string arguments;
	std::vector<std::string>	args;

	std::string clMsg = cleanMsg(msg);
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

	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	return std::tuple<AClient &, std::string, std::vector<std::string> >(user, cmd, args);
}
