/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/17 17:27:22 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/22 20:05:19 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"
#include <vector>
#include <string>
#include <unistd.h>

e_command	mapToEnum(std::string cmd){
	if (cmd == "USER") return CMD_USER;
	if (cmd == "NICK") return CMD_NICK;
	if (cmd == "PASS") return CMD_PASS;
	if (cmd == "PING") return CMD_PING;
	if (cmd == "QUIT") return CMD_QUIT;
	if (cmd == "PRIVMSG") return CMD_PRIVMSG;
	if (cmd == "LIST") return CMD_LIST;
	if (cmd == "JOIN") return CMD_JOIN;
	if (cmd == "WHO") return CMD_WHO;
	if (cmd == "WHOIS") return CMD_WHOIS;
	if (cmd == "PART") return CMD_PART;
	if (cmd == "INVITE") return CMD_INVITE;
	if (cmd == "TOPIC") return CMD_TOPIC;
	if (cmd == "MODE") return CMD_MODE;
	if (cmd == "OPER") return CMD_OPER;
	if (cmd == "KILL") return CMD_KILL;
	return CMD_UNKNOWN;
}

void Command::parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server){
	e_command command = mapToEnum(cmd);
	
	if (command > CMD_SIZE_OPEN_INT && command < CMD_SIZE_REGISTERED_INT){
		if (!user.getIsRegistered()){
			user.userNotRegisteredMsg(cmd);
			return;
		}
	}
	if (command > CMD_SIZE_REGISTERED_INT && command < CMD_SIZE_OPER_INT){
		if (!user.getIsOperator()){
			user.userNotOperatorMsg(cmd);
			return;
		}
	}
	
	switch (command) {
		case CMD_USER:	Command::user(user, cmd, args); 						break;
		case CMD_NICK:	Command::nick(user, cmd, args, server->getClientList());break;
		case CMD_PASS:	Command::password(user, cmd, args, server); 			break;
		case CMD_PING:	Command::ping(user, cmd, args, server);					break;
		case CMD_QUIT:	Command::quit(user, cmd, args, server);					break;
		case CMD_PRIVMSG:	server->sendPrivMsg(&user, args);					break;
		case CMD_LIST:	server->sendChannelList(&user);							break;
		case CMD_JOIN:	server->joinChannel(user, args);						break;
		case CMD_WHO:	server->sendWho(&user, args[0]);						break;
		case CMD_WHOIS:	server->sendWhoIs(&user, args[0]);						break;
		case CMD_PART:	server->partChannel(&user, args[0]);					break;
		case CMD_INVITE:server->sendInvite(&user, args);						break;
		case CMD_TOPIC:	server->setChannelTopic(user, args);					break;
		case CMD_MODE:	server->setChannelMode(user, args);						break;
		case CMD_OPER:	Command::oper(user, cmd, args, server);					break;
		case CMD_KILL:	Command::kill(user, cmd, args, server);					break;
		case CMD_SIZE_OPEN:														break;
		case CMD_SIZE_REGISTERED:												break;
		case CMD_SIZE_OPER:														break;
		case CMD_UNKNOWN:														break;
	}

}

/*
error 	C_LRED text + C_RESET variables + C_LRED rest of the text
message C_RESET text + C_LCYAN variables + C_RESET rest of the text
Joining C_RESET text + C_CYAN variables + C_RESET rest of the text
leaving C_RED text + C_RESET variables + C_RED rest of the text

sendMsg --> never forget \r\n, or never forgive
*/

/* ************************************************************************** *\
*																			  *
*																			  *
*				Static Functions											  *
*																			  *
*																			  *		
\* ************************************************************************** */

/* ************************************************************************** *\
*				USER														  *
\* ************************************************************************** */
static void	Command::user(Client &user, const std::string& cmd, const std::vector<std::string> &args) {

	if (user.getIsRegistered()){
		user.sendMsg("462 " + user.getNickName() + " " + ERR_ALREADYREGISTERED);
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_LRED "User " C_RESET << user.getNickName() 
						<< C_LRED " is already registered"	C_RESET << std::endl;
		return ;
	}

	if (args.size() < 4){
		user.sendMsg("461 " + user.getBestName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_LRED << "Error: format required: USER <user name> * <host> :<realname>" 
						<< C_RESET << std::endl;
		return ;
	}

	user.setIdentName(args[0]);
	if (args[1] != "*"){
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_LRED << "Error: format required: USER <user name> * <host> :<realname>" 
						<< C_RESET << std::endl;
		return ;
	}
	user.setServer(args[2]);
	
	std::string temp = args[3].substr(0);
	if (args.size() > 4){
		for (int i = 4; i < args.size(); i++)
			temp.append(" " + args[i]);
	}
	user.setRealName(temp);
	
	user.userRegistration();
}

/* ************************************************************************** *\
*				PASS														  *
\* ************************************************************************** */
static void Command::password(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server) {

	std::string serverName = std::getenv("IRC_SERVNAME");
	if (args.empty() || args[0].empty()){
		user.sendMsg(":" + serverName + "461 " + user.getBestName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_LRED << "No imput or empty input. Password not submitted, please provide a password"
						<< C_RESET << std::endl;
		return ;
	}
	if (user.getIsRegistered() == true){
		user.sendMsg(":" + serverName + "462 " + user.getNickName() + " " + ERR_ALREADYREGISTERED);
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_LRED << "User " C_RESET << user.getNickName() 
						<< C_LRED << " is already registered" C_RESET << std::endl;
		return ;
	}
	
	if (server->validatePassword(args[0]) != 1 && \
		server->validatePassword(args[0]) != 2 ){
		user.sendMsg(":" + serverName + "464 " + user.getBestName() + " " + ERR_PASSWDMISMATCH);
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_LRED "Wrong password " C_RESET
						<< user.getBestName() 
						<< C_LRED << " disconnected from server" C_RESET << std::endl;
		close (user.getPollInfofd());
		user.setPollInfofd(-1); 
		return ;
	}
	if (server->validatePassword(args[0]) == 2)
		user.setIsOperator(true);
	user.setHasPassword(true);
	user.userRegistration();
}

/* ************************************************************************** *\
*				NICK														  *
\* ************************************************************************** */
static void Command::nick(Client &user, const std::string& cmd, const std::vector<std::string> &params, std::vector<Client*> clients) {
	
	std::string nickname = params[0];

	std::string serverName = std::getenv("IRC_SERVNAME");
	if (nickname.empty()) {
		user.sendMsg(":" + serverName + " 431 * " + user.getBestName() + " " + ERR_NONICKNAMEGIVEN);
		return ;
	}
	for (std::vector<Client *>::const_iterator i = clients.begin(); i != clients.end(); ++i)
		if ((*i)->getNickName() == nickname && (*i) != &user){
			user.sendMsg("433 * " + nickname + " " +  ERR_NICKNAMEINUSE);
			return ;
		}
	if (isdigit(nickname.at(0))){
		user.sendMsg(":432 " + serverName + " 432 * " + nickname + " " + ERR_ERRONEUSNICKNAME);
		if (verboseCheck() >= V_USER)
			std::cout 	<< "Wrong character in name " << nickname
						<< " First character in nick name cannot be a digit" << std::endl;
		return ;
	}
	for (int i = 1; i < nickname.size(); i++){
		if (!isalnum(nickname[i])){
			user.sendMsg(":432 " + serverName + "432 * " + nickname + " " + ERR_ERRONEUSNICKNAME);
			if (verboseCheck() >= V_USER)
			std::cout 	<< "Wrong character in name " << nickname
						<< " Only alphanumeric characters are allowed in nick name" << std::endl;
			return ;
		}
	}
	user.setNickName(nickname);
	user.userRegistration();
}

/* ************************************************************************** *\
*				PING														  *
\* ************************************************************************** */
static void Command::ping(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.empty() || args[0].empty()){
		std::string serverName = std::getenv("IRC_SERVNAME");
		user.sendMsg(":" + serverName + "461 * " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	server->sendPong(&user, args[0]);
}

/* ************************************************************************** *\
*				QUIT														  *
\* ************************************************************************** */
#include <fcntl.h>
static void	Command::quit(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (!args[0].empty()){
		user.sendMsg(":" + user.getBestName() + "!~" + user.getUserName() + "@" + user.getIpHostName() + " " \
						+ cmd + ":Client Quit " + args[0] + "\r\n");
		user.sendMsg("ERROR :Closing Link: " + user.getIpHostName() + " (Client Quit)\r\n");
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getBestName() 
						<< " is exiting the network with the message: " + args[0] << std::endl;
	} else {
		user.sendMsg(":" + user.getBestName() + "!~" + user.getUserName() + "@" + user.getIpHostName() + " " \
						+ cmd + ":Client Quit\r\n");
		user.sendMsg("ERROR :Closing Link: " + user.getIpHostName() + " (Client Quit)\r\n");
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getBestName() 
						<< " is exiting the network" << std::endl;
	}
	
	close (user.getPollInfofd());
	user.setPollInfofd(-1); 
}

/* ************************************************************************** *\
*				OPER														  *
\* ************************************************************************** */
static void	Command::oper(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (args.size() != 2){
		user.sendMsg(":" + serverName + " 461 " + user.getBestName() + " " + cmd + ERR_NEEDMOREPARAMS);
		if (verboseCheck() >= V_ADMIN)
			std::cout 	<< "User " + user.getNickName() + " tried to use OPER with incomplete parameters."	<< std::endl;
		return ;
	}
		
	if (server->validatePassword(args[1]) != 2){
		user.sendMsg(":" + serverName + " 464 * " + user.getBestName() + " " + cmd + ERR_PASSWDMISMATCH);
		return ;
	}
	if (user.getIsOperator() == false){
		// user.sendMsg(user.getBestName() + " :Admin privileges required");
		if (verboseCheck() >= V_ADMIN)
			std::cout 	<< "User " << user.getNickName() 
						<< " is not IRC Operator : Admin privileges required"	<< std::endl;
		return ;
	}
	
	const std::vector<Client *> &clientList = server->getClientList();

	for (std::vector<Client *>::const_iterator it = clientList.begin(); it != clientList.end(); ++it) {
		std::cout << C_MGNT << (*it)->getNickName() << std::endl;
		if ((*it)->getNickName() == args[0] && user.getIsOperator() == true){
			(*it)->setIsOperator(true);
			user.sendMsg(":" + serverName + " 381 * " + (*it)->getNickName() + " " + RPL_YOUREOPER);
			//set Mode +o
			break;
		}
		if (it == server->getClientList().end()){
				user.sendMsg(":" + serverName + " 491 " + (*it)->getNickName() + " " + cmd + ERR_NOOPERHOST);
				if (verboseCheck() >= V_USER)
					std::cout 	<< "Request rejected " << std::getenv("IRC_SERVNAME")
								<< " doesn’t allow connections from current network of user "
								<< (*it)->getNickName()	<< std::endl;
				return ;
		}
	}
}

/* ************************************************************************** *\
*				KILL														  *
\* ************************************************************************** */
static void	Command::kill(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (args.size() != 2){
		user.sendMsg(":" + serverName + " 461 " + user.getNickName() + " " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
		
	if (user.getIsOperator() == false){
		user.sendMsg("461 " + user.getNickName() + " " + cmd + ERR_NOPRIVILEGES);
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getNickName() 
						<< " Permission Denied - You're not an IRC operator"	<< std::endl;
		return ;
	}
	
	const std::vector<Client *> &clientList = server->getClientList();

	for (std::vector<Client *>::const_iterator it = clientList.begin(); it != clientList.end(); ++it) {
		if ((*it)->getNickName() == args[0]){
			user.sendMsg("ERROR :Closing Link: " + serverName + " Killed " + \
						(*it)->getNickName() + ": " + args[args.size()] + "\r\n");
			close ((*it)->getPollInfofd());
			(*it)->setPollInfofd(-1);
			if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << (*it)->getNickName()
						<< " has been killed; reason :"	
						<< args[args.size()] << std::endl;
			break;
		}
		if (it == server->getClientList().end()){
				user.sendMsg(":" + serverName + " 491 " + (*it)->getNickName() + " " + cmd + ERR_NOOPERHOST);
				if (verboseCheck() >= V_USER)
					std::cout 	<< "User " << (*it)->getNickName()
								<< " has been killed; reason :"	
								<< args[args.size()] << std::endl;
				return ;
		}
	}
}

