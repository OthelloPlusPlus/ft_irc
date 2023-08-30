/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/17 17:27:22 by emlicame      #+#    #+#                 */
/*   Updated: 2023/08/30 15:04:12 by ohengelm      ########   odam.nl         */
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

// void Command::parseCmd(Client &user, const std::string& cmd, const std::string& params){
void Command::parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args, std::vector<Client*> clients){

	if (cmd == "USER")
		Command::user(user, cmd, args);
	else if (cmd == "PASS")
		Command::password(user, cmd, args);
	else if (cmd == "NICK")
		Command::nick(user, cmd, args, clients);
	else if (cmd == "PING")
		Command::ping(user, cmd, args);
}

// void Command::parseMsg(Client &user){
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
			parseCmd(user, command, args, server->getClientList());
		}
		//change!! no more cmd and args, if I pass only cmd and no space, if condition spacePos != std::string::npos fails and nothing happens
	}
}

// // void Command::parseMsg(Client &user){
// void Command::parseMsg(Client &user, std::vector<Client*>& clients){
// 	// Command::cleanMsg(user);
// 	std::vector<std::string>	cmd;
// 	cmd = ircSplitMulti(user.getBuff(), "\r\n");
// 	for (std::vector<std::string>::const_iterator it = cmd.begin(); it != cmd.end(); ++it) {
// 		const std::string& element = *it;
// 		size_t spacePos = element.find(' ');
// 		if (spacePos != std::string::npos) {
// 			std::string command = element.substr(0, spacePos);
// 	   	 	std::string params = element.substr(spacePos + 1);
// 			// parseCmd(user, command, params);
// 			std::vector<std::string>	args;
// 			args = ircSplit(params, " ");
// 			parseCmd(user, command, args, clients);
// 		}
// 		//change!! no more cmd and args, if I pass only cmd and no space, if condition spacePos != std::string::npos fails and nothing happens
// 	}
// }


// void Command::user(Client &user, const std::string& cmd, const std::string &params) {
// 	size_t space1Pos = params.find(' ', 0);
// 	size_t space2Pos = params.find(' ', space1Pos + 1); 
// 	size_t colonPos = params.find(':', space2Pos);
// 	if (space1Pos != std::string::npos && space2Pos != std::string::npos && colonPos != std::string::npos) {
// 		std::string idenName = params.substr(0, space1Pos);
// 		std::string server = params.substr(space2Pos + 1 , colonPos - space2Pos - 2);
// 		std::string realName = params.substr(colonPos + 1);
		
// 		// Set the class attributes
// 		user.setIdentName(idenName);
// 		user.setServer(server);
// 		user.setRealName(realName);
// 	}
// }

// // void Command::nick(Client &user, const std::string& cmd, const std::string &params) {
// 		user.setNickName(params.substr(0));
// }

// void Command::password(Client &user, const std::string& cmd, const std::string& params) {
// 	if (params.at(0) == ':')
// 		user.setPassword(params.substr(1));
// 	else
// 		user.setPassword(params.substr(0));
// }


/*
PASS :Gatto
USER Emanuela_De_La_Vega * 10.11.1.15 :Marylin vos Savant
NICK Magic
PRIVMSG Bot :a
PING 1692972425


nc 10.11.2.7 6667
*/