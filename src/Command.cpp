/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/17 17:27:22 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/02 18:39:03 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"
#include <vector>
#include <string>
#include <unistd.h>
//	int	close(int filedes);
#include <algorithm>
// std::transform

e_command	mapToEnum(std::string cmd){
	if (cmd == "USER") return CMD_USER;
	if (cmd == "NICK") return CMD_NICK;
	if (cmd == "PASS") return CMD_PASS;
	if (cmd == "PING") return CMD_PING;
	if (cmd == "QUIT") return CMD_QUIT;
	if (cmd == "AWAY") return CMD_AWAY;
	if (cmd == "SEND") return CMD_SEND;
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
	if (cmd == "") return CMD_EMPTY;
	return CMD_UNKNOWN;
}

void Command::parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args){
	if (args.size() == 0)
		return ;
		
	e_command command = mapToEnum(cmd);
	
	if (command > CMD_SIZE_OPEN_INT && command < CMD_SIZE_REGISTERED_INT){
		if (!user.getIsRegistered()){
			userNotRegisteredMsg(user, cmd);
			return;
		}
	}

	if (command > CMD_SIZE_REGISTERED_INT && command < CMD_SIZE_OPER_INT){
		if (!user.getIsOperator()){
			userNotOperatorMsg(user, cmd);
			return;
		}
	}

	switch (command) {
		case CMD_USER:		Command::user(user, cmd, args);					break;
		case CMD_NICK:		Command::nick(user, cmd, args);					break;
		case CMD_PASS:		Command::password(user, cmd, args);				break;
		case CMD_PING:		Command::ping(user, cmd, args);					break;
		case CMD_QUIT:		Command::quit(user, cmd, args);					break;
		case CMD_AWAY:		Command::away(user, cmd, args);					break;
		case CMD_SEND:		Command::sendFile(user, cmd, args);				break;
		case CMD_PRIVMSG:	user.getServer()->sendPrivMsg(user, args);		break;
		case CMD_LIST:		user.getServer()->sendChannelList(user);		break;
		case CMD_JOIN:		user.getServer()->joinChannel(user, args);		break;
		case CMD_WHO:		user.getServer()->sendWho(user, args[0]);		break;
		case CMD_WHOIS:		user.getServer()->sendWhoIs(user, args[0]);		break;
		case CMD_PART:		user.getServer()->partChannel(user, args[0]);	break;
		case CMD_INVITE:	user.getServer()->sendInvite(user, args);		break;
		case CMD_TOPIC:		user.getServer()->setChannelTopic(user, args);	break;
		case CMD_MODE:		user.getServer()->setChannelMode(user, args);	break;
		case CMD_OPER:		Command::oper(user, cmd, args);					break;
		case CMD_KILL:		Command::kill(user, cmd, args);					break;
		case CMD_EMPTY:														break;
		case CMD_SIZE_OPEN:													break;
		case CMD_SIZE_REGISTERED:											break;
		case CMD_SIZE_OPER:													break;
		case CMD_UNKNOWN:	Command::unknownCmd(user, cmd);					break;
	}
}


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

	std::string serverName = std::getenv("IRC_SERVNAME");
	if (user.getIsRegistered()){
		user.sendMsg(":" + serverName + user.getNickName() + " " + ERR_ALREADYREGISTERED);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User "
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is already registered"
						<<	C_RESET	<<	std::endl;
		return ;
	}

	if (args.size() < 4){
		user.sendMsg(":" + serverName + user.getBestName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Error: format required: USER <user name> * <host> :<realname>" 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	user.setUserName(args[0]);
	if (args[1] != "*"){
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Error: format required: USER <user name> * <host> :<realname>" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
	// std::cout	<< __func__	<< __LINE__	<< args[2]	<< std::endl;
	// user.setClientIP(args[2]);
	// std::cout	<< __func__	<< __LINE__	<< user.getClientIP()	<< std::endl;
	
	std::string temp = args[3].substr(0);
	if (args.size() > 4){
		for (int i = 4; i < args.size(); i++)
			temp.append(" " + args[i]);
	}
	user.setRealName(temp);
	
	user.setIsRegistered(true);
}

/* ************************************************************************** *\
*				PASS														  *
\* ************************************************************************** */
static void Command::password(Client &user, const std::string& cmd, const std::vector<std::string>& args) {

	std::string serverName = std::getenv("IRC_SERVNAME");
	if (args.empty() || args[0].empty()){
		user.sendMsg(":" + serverName + user.getBestName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Password not submitted. No imput provided"
						<<	C_RESET	<<	std::endl;
			
		return ;
	}
	if (user.getIsRegistered() == true){
		user.sendMsg(":" + serverName + user.getNickName() + " " + ERR_ALREADYREGISTERED);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User "
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is already registered"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	
	if (user.getServer()->validatePassword(args[0]) == 0 ){
		user.sendMsg(":" + serverName + user.getBestName() + " " + ERR_PASSWDMISMATCH);
		if (verboseCheck()	>= V_USER)
			std::cerr	<<	C_LRED	<<	"Wrong password " 
						<<	C_RESET	<<	user.getBestName() 
						<<	C_LRED	<<	" disconnected from server" 
						<<	C_RESET	<<	std::endl;
		user.closeFD();
		return ;
	}
	if (user.getServer()->validatePassword(args[0]) == 2)
		user.setIsOperator(true);
	user.passwordValidation(true);
	user.setIsRegistered(true);
}

/* ************************************************************************** *\
*				NICK														  *
\* ************************************************************************** */
static void Command::nick(Client &user, const std::string& cmd, const std::vector<std::string> &args) {
	
	std::string nickname = args[0];
	std::vector<AClient*> clients = user.getServer()->getClientList();
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (nickname.empty()) {
		user.sendMsg(":" + serverName + ERR_NONICKNAMEGIVEN);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Nick name not provided " 
						<<	C_RESET	<<	user.getBestName() 
						<<	C_LRED	<<	" No imput provided" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
	
	AClient	*clientName = user.getServer()->getClient(nickname);
	// for (std::vector<AClient *>::const_iterator i = clients.begin(); i != clients.end(); ++i){
		// std::string	clientName = (*i)->getNickName();
		// if (clientName == nickname && (*i) != &user){
	if (clientName != nullptr && clientName != &user){
		user.sendMsg(":" + serverName + " 433 * " + nickname + " " +  ERR_NICKNAMEINUSE);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Nick name not available. [" 
						<<	C_RESET	<<	nickname
						<<	C_LRED	<<	"] is already in use on the network." 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	if (isdigit(nickname.at(0))){
		user.sendMsg(":" + serverName + " 432 * " + nickname + " " + ERR_ERRONEUSNICKNAME);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Wrong character in name [" 
						<<	C_RESET	<<	nickname
						<<	C_LRED	<<	"] First character in nick name cannot be a digit" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
	for (int i = 1; i < nickname.size(); i++){
		if (!isalnum(nickname[i])){
			user.sendMsg(":" + serverName + "432 * " + nickname + " " + ERR_ERRONEUSNICKNAME);
			if (verboseCheck()	>= V_USER)
				std::cout	<<	C_LRED	<<	"Wrong character in name [" 
							<<	C_RESET	<<	nickname
							<<	C_LRED	<<	"] Only alphanumeric characters are allowed in nick name" 
							<<	C_RESET	<<	std::endl;
			return ;
		}
	}
	user.setNickName(nickname);
	user.setIsRegistered(true);
}

/* ************************************************************************** *\
*				PING														  *
\* ************************************************************************** */
static void Command::ping(Client &user, const std::string &cmd, const std::vector<std::string> &args){
	if (args.empty() || args[0].empty()){
		std::string serverName = std::getenv("IRC_SERVNAME");
		user.sendMsg(":" + serverName + "461 * " + cmd + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"No imput provided. Command " 
						<<	C_RESET	<<	cmd
						<<	C_LRED	<<	" needs a token as unique identifier" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
	user.getServer()->sendPong(user, args[0]);
}

/* ************************************************************************** *\
*				QUIT														  *
\* ************************************************************************** */
#include <fcntl.h>
static void	Command::quit(Client &user, const std::string &cmd, const std::vector<std::string> &args){
	
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (!args[0].empty()){
		user.sendMsg(":" + user.getBestName() + "!~" + user.getUserName() + "@" + user.getClientIP() + " " \
						+ cmd + ":Client Quit " + args[0] + "\r\n");
		user.sendMsg("ERROR :Closing Link: " + user.getClientIP() + " (Client Quit)\r\n");
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_RESET	<<	"User "
						<<	C_LCYAN	<<	user.getBestName()
						<<	C_RESET	<<	" is exiting the network with the message: "
						<<	C_LCYAN	<<	args[0]
						<<	C_RESET	<<	std::endl;
	} else {
		user.sendMsg(":" + user.getBestName() + "!~" + user.getUserName() + "@" + user.getClientIP() + " " \
						+ cmd + ":Client Quit\r\n");
		user.sendMsg("ERROR :Closing Link: " + user.getClientIP() + " (Client Quit)\r\n");
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_RESET	<<	"User "
						<<	C_LCYAN	<<	user.getBestName()
						<<	C_RESET	<<	" is exiting the network"
						<<	C_RESET	<<	std::endl;
	}
	user.closeFD();
}


/* ************************************************************************** *\
*				AWAY														  *
\* ************************************************************************** */

static void Command::away(Client &user, const std::string &cmd, const std::vector<std::string> &args){
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (!args[0].empty()){
		user.sendMsg(":" + serverName + " " + user.getBestName()+ " " + RPL_NOWAWAY);
		if (verboseCheck()	>= V_USER)
				std::cout	<<	C_RESET	<<	"User "
							<<	C_LCYAN	<<	user.getBestName()
							<<	C_RESET	<<	" is away"
							<<	C_RESET	<<	std::endl;
	}
}

/* ************************************************************************** *\
*				OPER														  *
\* ************************************************************************** */
static void	Command::oper(Client &user, const std::string &cmd, const std::vector<std::string> &args){
	
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (args.size() != 2){
		user.sendMsg(":" + serverName + " 461 * " + user.getNickName() + " " + cmd + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"No imput provided. User " 
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" tried to use OPER with incomplete set of parameters"
						<<	C_RESET	<<	std::endl;
		return ;
	}
		
	if (user.getServer()->validatePassword(args[1]) != 2){
		user.sendMsg(":" + serverName + " 464 * " + user.getNickName() + " " + cmd + ERR_PASSWDMISMATCH);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Wrong password. Command " 
						<<	C_RESET	<<	cmd 
						<<	C_LRED	<<	" requiers Admin password"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	if (user.getIsOperator() == false){
		user.sendMsg(":" + serverName + " 464 * " + user.getNickName() + " " + cmd + ERR_PASSWDMISMATCH);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User " 
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is not IRC Operator : Admin privileges required"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	
	AClient	*clientName = user.getServer()->getClient(args[0]);
	const std::vector<AClient *> &clientList = user.getServer()->getClientList();
	// for (std::vector<AClient *>::const_iterator it = clientList.begin(); it != clientList.end(); ++it) {
		// if ((*it)->getNickName() == args[0] && user.getIsOperator() == true){
	if (clientName != nullptr && user.getIsOperator() == true) {
		clientName->setIsOperator(true);
		user.sendMsg(":" + serverName + " 381 * " + clientName->getNickName() + " " + RPL_YOUREOPER);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_RESET	<<	"User "
						<<	C_LCYAN	<<	clientName->getNickName()
						<<	C_RESET	<<	" is now IRC server Operator"
						<<	C_RESET	<<	std::endl;
	}
	if (clientName == nullptr){
		user.sendMsg(":" + serverName + " 491 * " + args[0] + " " + cmd + ERR_NOOPERHOST);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Request rejected  " 
						<<	C_RESET	<<	serverName
						<<	C_LRED	<<	" doesn’t allow connections from current network of user "
						<<	C_RESET	<<	args[0] <<	std::endl;
		return ;
	}
}

/*
PASS password
USER Ema * 123 :Lic
NICK Mag
*/
/* ************************************************************************** *\
*				KILL														  *
\* ************************************************************************** */
static void	Command::kill(Client &user, const std::string &cmd, const std::vector<std::string> &args){
	
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (args.size() != 2){
		user.sendMsg(":" + serverName + " 461 * " + user.getNickName() + " " + cmd + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"Target not submitted. No imput provided" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
		
	if (user.getIsOperator() == false){
		user.sendMsg(":" + serverName + " 481 * " + user.getNickName() + " " + cmd + ERR_NOPRIVILEGES);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User " 
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is not IRC Operator : Admin privileges required"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	
	AClient	*clientName = user.getServer()->getClient(args[0]);
	const std::vector<AClient *> &clientList = user.getServer()->getClientList();
	if (clientName != nullptr){
		user.sendMsg("ERROR :Closing Link: " + serverName + " Killed " + \
					clientName->getNickName() + ": " + args[args.size()] + "\r\n");
		clientName->closeFD();
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User " 
						<<	C_RESET	<<	clientName->getNickName()
						<<	C_LRED	<<	" has been killed; reason : "
						<<	C_RESET	<<	args[args.size()] << std::endl;
	}
	if (clientName == nullptr){
			user.sendMsg(":" + serverName + " 402 * " + args[0] + " " + ERR_NOSUCHSERVER);
			if (verboseCheck() >= V_USER)
				std::cout 	<<	C_LRED	<<	"Server or user "
							<<	C_RESET	<<	args[0]
							<<	C_LRED	<<	" targeted to be killed, does not exist"	
							<<	C_RESET	<<	std::endl;
			return ;
	}
}

static void	Command::unknownCmd(Client &user, const std::string &cmd){
	std::string serverName = std::getenv("IRC_SERVNAME");
	user.sendMsg(":" + serverName + " 421 " + user.getBestName() + " " + cmd + ERR_UNKNOWNCOMMAND);
	if (verboseCheck()	>= V_USER)	
		std::cout	<<	C_LRED	<<	"The command typed is unknown" 
					<<	C_RESET	<<	std::endl;
	return ;
}


/* ************************************************************************** *\
*				SEND - File Transfer														  *
\* ************************************************************************** */

#include <fstream>

static void	Command::sendFile(Client &user, const std::string &cmd, const std::vector<std::string> &args){
	std::string serverName = std::getenv("IRC_SERVNAME");

	FileTransfer fileTransfer;
	
	if (args.size() < 4){
		user.sendMsg(":" + serverName + " 461 " + user.getNickName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"No imput provided. File transfer requires 3 parameters <file name> <host-IP address> <port>" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
	if (args.size() >= 4) {
			fileTransfer.targetName = args[0];
			fileTransfer.fileName = args[1];
			fileTransfer.hostTarget = user.getServer()->getTransferIP();
			fileTransfer.port = 59;

			user.fileTransfers.push_back(fileTransfer);
	
		}
		size_t	pos = fileTransfer.fileName.find_last_of('/');
		std::string	fileName = fileTransfer.fileName.substr(pos + 1);
		std::cout << C_MGNT << fileName << C_RESET <<std::endl;
		
		std::ifstream file(fileName, std::ios::binary);
		if (!file) {
			user.sendMsg(":" + serverName + " 402 " + fileTransfer.targetName + " " + ":Invalid file path\r\n");
			return ;
		}
		// Read and send the file in chunks
		const int bufferSize = 1024;
		char buffer[bufferSize];
		// while (!file.eof()) {
		// 	file.read(buffer, bufferSize);
		// 	send(user.getPollInfofd(), buffer, file.gcount(), 0);
		// }
		file.close();
}

static void	Command::userNotRegisteredMsg(Client &user, std::string cmd){
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (verboseCheck() >= V_USER)
		std::cout 	<< C_LRED << serverName << " User " << C_RESET << user.getBestName() 
					<< C_LRED " needs to be registered for the " << C_RESET << cmd 
					<< C_LRED " command" << C_RESET << std::endl;
}

static void	Command::userNotOperatorMsg(Client &user, std::string cmd){
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (verboseCheck() >= V_USER)
		std::cout 	<< C_LRED << serverName << "User " << C_RESET << user.getBestName() 
					<< C_LRED " needs to be operator for the " << C_RESET << cmd 
					<< C_LRED " command" << C_RESET << std::endl;
}