/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/24 20:22:20 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/24 20:22:23 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"
#include "ansicolors.hpp"
#include <string>
// std::string
#include <vector>
// std::vector
#include <unistd.h>
// int close(int filedes);
#include <algorithm>
// std::transform
// #include <fcntl.h>
#include <fstream>
// std::ifstream
// std::fstream
#include <sstream>
//  std::stringstream

static void userName(AClient &user, const std::vector<std::string> &args);
static void password(AClient &user, const std::vector<std::string> &args);
static void nick(AClient &user, const std::vector<std::string> &args);
static void ping(AClient &user, const std::vector<std::string> &args);
static void quit(AClient &user, const std::vector<std::string> &args);
static void away(AClient &user, const std::vector<std::string> &args);
static void oper(AClient &user, const std::vector<std::string> &args);
static void kill(AClient &user, const std::vector<std::string> &args);
static void send(AClient &user, const std::vector<std::string> &args);
static void accept(AClient &user, const std::vector<std::string> &args);
static void reject(AClient &user, const std::vector<std::string> &args);

static void unknownCmd(AClient &user, const std::string &cmd);

static void userNotRegisteredMsg(AClient &user, std::string cmd);
static void userNotOperatorMsg(AClient &user, std::string cmd);

e_command	mapToEnum(std::string cmd){
	if (cmd == "USER") return CMD_USER;
	if (cmd == "NICK") return CMD_NICK;
	if (cmd == "PASS") return CMD_PASS;
	if (cmd == "PING") return CMD_PING;
	if (cmd == "QUIT") return CMD_QUIT;
	if (cmd == "AWAY") return CMD_AWAY;
	if (cmd == "SEND") return CMD_SEND;
	if (cmd == "ACCEPT") return CMD_ACCEPT;
	if (cmd == "REJECT") return CMD_REJECT;
	if (cmd == "PRIVMSG") return CMD_PRIVMSG;
	if (cmd == "LIST") return CMD_LIST;
	if (cmd == "JOIN") return CMD_JOIN;
	if (cmd == "WHO") return CMD_WHO;
	if (cmd == "WHOIS") return CMD_WHOIS;
	if (cmd == "PART") return CMD_PART;
	if (cmd == "INVITE") return CMD_INVITE;
	if (cmd == "TOPIC") return CMD_TOPIC;
	if (cmd == "MODE") return CMD_MODE;
	if (cmd == "NAMES") return CMD_NAMES;
	if (cmd == "KICK") return CMD_KICK;
	if (cmd == "NOTICE") return CMD_NOTICE;
	if (cmd == "OPER") return CMD_OPER;
	if (cmd == "KILL") return CMD_KILL;
	if (cmd == "") return CMD_EMPTY;
	return CMD_UNKNOWN;
}

void Command::parseCmd(AClient &user, const std::string& cmd, const std::vector<std::string>& args) {

	if (args.size() == 0)
		return ;
		
	e_command command = mapToEnum(cmd);
	
	if (command > CMD_SIZE_OPEN_INT && command < CMD_SIZE_REGISTERED_INT) {
		if (!user.getIsRegistered()){
			userNotRegisteredMsg(user, cmd);
			return;
		}
	}

	if (command > CMD_SIZE_REGISTERED_INT && command < CMD_SIZE_OPER_INT) {
		if (!user.getIsOperator()){
			userNotOperatorMsg(user, cmd);
			return;
		}
	}

	switch (command) {
		case CMD_USER:		userName(user, args);							break;
		case CMD_NICK:		nick(user, args);								break;
		case CMD_PASS:		password(user, args);							break;
		case CMD_PING:		ping(user, args);								break;
		case CMD_QUIT:		quit(user, args);								break;
		case CMD_AWAY:		away(user, args);								break;
		case CMD_SIZE_OPEN:													break;
		
		case CMD_SEND:		send(user, args);								break;
		case CMD_ACCEPT:	accept(user, args);								break;
		case CMD_REJECT:	reject(user, args);								break;
		case CMD_PRIVMSG:	user.getServer()->sendPrivMsg(user, args);		break;
		case CMD_LIST:		user.getServer()->sendChannelList(user);		break;
		case CMD_JOIN:		user.getServer()->joinChannel(user, args);		break;
		case CMD_WHO:		user.getServer()->sendWho(user, args[0]);		break;
		case CMD_WHOIS:		user.getServer()->sendWhoIs(user, args[0]);		break;
		case CMD_PART:		user.getServer()->partChannel(user, args[0]);	break;
		case CMD_INVITE:	user.getServer()->sendInvite(user, args);		break;
		case CMD_TOPIC:		user.getServer()->setChannelTopic(user, args);	break;
		case CMD_MODE:		user.getServer()->setChannelMode(user, args);	break;
		case CMD_NAMES:		user.getServer()->sendNames(user, args);		break;
		case CMD_KICK:		user.getServer()->kickUser(user, args);			break;
		case CMD_NOTICE:	user.getServer()->sendNotice(user, args);		break;
		case CMD_SIZE_REGISTERED:											break;

		case CMD_OPER:		oper(user, args);								break;
		case CMD_KILL:		kill(user, args);								break;
		case CMD_SIZE_OPER:													break;

		case CMD_EMPTY:														break;
		case CMD_UNKNOWN:	unknownCmd(user, cmd);							break;
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

static void	userName(AClient &user, const std::vector<std::string> &args) {

	if (user.getIsRegistered()) {
		user.sendMsg(":" + *user.getServer() + " 462 " + user.getNickName() + ERR_ALREADYREGISTERED);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User "
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is already registered"
						<<	C_RESET	<<	std::endl;
		return ;
	}

	if (args.size() < 4) {
		user.sendMsg(":" + *user.getServer() + " 461 " + user.getBestName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Error: format required: USER <user name> * <host> :<realname>" 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	user.setUserName(args[0]);
	if (args[1] != "*") {
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Error: format required: USER <username> * <host> :<realname>" 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	std::string temp = args[3].substr(0);
	if (args.size() > 4){
		for (size_t i = 4; i < args.size(); i++)
			temp.append(" " + args[i]);
	}

	user.setRealName(temp);
	user.setIsRegistered(true);

}

/* ************************************************************************** *\
*				PASS														   *
\* ************************************************************************** */

static void	password(AClient &user, const std::vector<std::string>& args) {

	if (args.empty() || args[0].empty()) {
		user.sendMsg(":" + *user.getServer() + " 461 " + user.getBestName() + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Password not submitted. No imput provided"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	if (user.getIsRegistered() == true) {
		user.sendMsg(":" + *user.getServer() + " 462 " + user.getNickName() + ERR_ALREADYREGISTERED);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User "
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is already registered"
						<<	C_RESET	<<	std::endl;
		return ;
	}

	if (user.getServer()->validatePassword(args[0]) == 0 ) {
		user.sendMsg(":" + *user.getServer() + " 464 " + user.getBestName() + " " + ERR_PASSWDMISMATCH);
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
*				NICK														   *
\* ************************************************************************** */

static void nick(AClient &user, const std::vector<std::string> &args) {

	std::string nickname = args[0];

	if (nickname.empty()) {
		user.sendMsg(":" + *user.getServer() + " 431 " + ERR_NONICKNAMEGIVEN);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Nick name not provided " 
						<<	C_RESET	<<	user.getBestName() 
						<<	C_LRED	<<	" No imput provided" 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	AClient	*clientName = user.getServer()->getClient(nickname);
	if (clientName != nullptr && clientName != &user) {
		user.sendMsg(":" + *user.getServer() + " 433 " + nickname +  ERR_NICKNAMEINUSE);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Nick name not available. [" 
						<<	C_RESET	<<	nickname
						<<	C_LRED	<<	"] is already in use on the network." 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	if (isdigit(nickname.at(0))) {
		user.sendMsg(":" + *user.getServer() + " 432 " + nickname  + ERR_ERRONEUSNICKNAME);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Wrong character in name [" 
						<<	C_RESET	<<	nickname
						<<	C_LRED	<<	"] First character in nick name cannot be a digit" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
	for (size_t i = 1; i < nickname.size(); i++) {
		if (!isalnum(nickname[i])){
			user.sendMsg(":" + *user.getServer() + "432 " + nickname + ERR_ERRONEUSNICKNAME);
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
*				PING														   *
\* ************************************************************************** */

static void ping(AClient &user, const std::vector<std::string> &args) {

	if (args.empty() || args[0].empty()) {

		user.sendMsg(":" + *user.getServer() + "461 PING " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"No imput provided. Command " 
						<<	C_RESET	<<	"PING"
						<<	C_LRED	<<	" needs a token as unique identifier" 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	user.getServer()->sendPong(user, args[0]);

}

/* ************************************************************************** *\
*				QUIT														   *
\* ************************************************************************** */

static void	quit(AClient &user, const std::vector<std::string> &args) {

	if (!args[0].empty()) {
		user.sendMsg(":" + user + " QUIT Client Quit " + args[0]);
		user.sendMsg("ERROR :Closing Link: " + user.getClientIP() + " (Client Quit)");
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_RESET	<<	"User "
						<<	C_LCYAN	<<	user.getBestName()
						<<	C_RESET	<<	" is exiting the network with the message: "
						<<	C_LCYAN	<<	args[0]
						<<	C_RESET	<<	std::endl;
	}
	else {
		user.sendMsg(":" + user + " QUIT Client Quit");
		user.sendMsg("ERROR :Closing Link: " + user.getClientIP() + " (Client Quit)");
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_RESET	<<	"User "
						<<	C_LCYAN	<<	user.getBestName()
						<<	C_RESET	<<	" is exiting the network"
						<<	C_RESET	<<	std::endl;
	}

	user.closeFD();

}

/* ************************************************************************** *\
*				AWAY														   *
\* ************************************************************************** */

static void away(AClient &user, const std::vector<std::string> &args) {

	if (args[0] == user.getAway())
		return ;
	user.setAway(args[0]);
	if (args[0].empty()) {
		user.sendMsg(':' + *user.getServer() + " 305 " + user.getBestName() + " :You are no longer marked as being away");
		if (verboseCheck() >= V_USER)
			std::cout	<< C_RESET	<< "User "
						<< C_LCYAN	<< user.getBestName()
						<< C_RESET	<< " is no longer away"
						<< C_RESET	<< std::endl;
	}
	else {
		user.sendMsg(":" + *user.getServer() + " 306 " + user.getBestName() + RPL_NOWAWAY);
		if (verboseCheck()	>= V_USER)
				std::cout	<< C_RESET	<< "User "
							<< C_LCYAN	<< user.getBestName()
							<< C_RESET	<< " is away: "
							<< C_LCYAN	<< args[0]
							<< C_RESET	<< std::endl;
	}
}

/* ************************************************************************** *\
*				OPER														   *
\* ************************************************************************** */

static void	oper(AClient &user, const std::vector<std::string> &args) {

	if (args.size() != 2){
		user.sendMsg(":" + *user.getServer() + " 461 " + user.getNickName() + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"No imput provided. User " 
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" tried to use OPER with incomplete set of parameters"
						<<	C_RESET	<<	std::endl;
		return ;
	}

	if (user.getServer()->validatePassword(args[1]) != 2) {
		user.sendMsg(":" + *user.getServer() + " 464 " + user.getNickName() + ERR_PASSWDMISMATCH);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Wrong password. Command " 
						<<	C_RESET	<<	"OPER" 
						<<	C_LRED	<<	" requires Admin password"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	if (user.getIsOperator() == false) {
		user.sendMsg(":" + *user.getServer() + " 464 " + user.getNickName() + ERR_PASSWDMISMATCH);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User " 
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is not IRC Operator : Admin privileges required"
						<<	C_RESET	<<	std::endl;
		return ;
	}

	AClient	*clientName = user.getServer()->getClient(args[0]);
	if (clientName == nullptr){
		user.sendMsg(":" + *user.getServer() + " 491 " + args[0] + ERR_NOOPERHOST);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Request rejected  " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" doesn’t allow connections from current network of user "
						<<	C_RESET	<<	args[0] <<	std::endl;
		return ;
	}
	else if (user.getIsOperator() == true) {
		clientName->setIsOperator(true);
		user.sendMsg(":" + *user.getServer() + " 381 " + clientName->getNickName() + RPL_YOUREOPER);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_RESET	<<	"User "
						<<	C_LCYAN	<<	clientName->getNickName()
						<<	C_RESET	<<	" is now IRC server Operator"
						<<	C_RESET	<<	std::endl;
	}
}

/* ************************************************************************** *\
*				KILL														   *
\* ************************************************************************** */

static void	kill(AClient &user, const std::vector<std::string> &args) {

	if (args.size() != 2) {
		user.sendMsg(":" + *user.getServer() + " 461 " + user.getNickName() + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"Target not submitted. No imput provided" 
						<<	C_RESET	<<	std::endl;
		return ;
	}
	if (user.getIsOperator() == false) {
		user.sendMsg(":" + *user.getServer() + " 481 " + user.getNickName() + ERR_NOPRIVILEGES);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User " 
						<<	C_RESET	<<	user.getNickName()
						<<	C_LRED	<<	" is not IRC Operator : Admin privileges required"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	AClient	*clientName = user.getServer()->getClient(args[0]);
	if (clientName != nullptr) {
		user.sendMsg("ERROR :Closing Link: " + *user.getServer() + " Killed " + clientName->getNickName() + ": " + args[args.size() - 1]);
		clientName->closeFD();
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"User " 
						<<	C_RESET	<<	clientName->getNickName()
						<<	C_LRED	<<	" has been killed; reason : "
						<<	C_RESET	<<	args[args.size() - 1] << std::endl;
	}
	else {
		user.sendMsg(":" + *user.getServer() + " 402 " + args[0] + " " + ERR_NOSUCHSERVER);
		if (verboseCheck() >= V_USER)
			std::cout 	<<	C_LRED	<<	"Server or user "
						<<	C_RESET	<<	args[0]
						<<	C_LRED	<<	" targeted to be killed, does not exist"
						<<	C_RESET	<<	std::endl;
		return ;
	}
}

/* ************************************************************************** *\
*				UNKNOWN CMD													   *
\* ************************************************************************** */

static void	unknownCmd(AClient &user, const std::string &cmd){

	user.sendMsg(":" + *user.getServer() + " 421 " + user.getBestName() + " " + cmd + ERR_UNKNOWNCOMMAND);
	if (verboseCheck()	>= V_USER)	
		std::cout	<<	C_LRED	<<	"The command '"
					<<	C_RED	<< cmd
					<<	C_LRED	<< "' is unknown" 
					<<	C_RESET	<<	std::endl;
	return ;
}

/* ************************************************************************** *\
*				MSG - User not registered									   *
\* ************************************************************************** */

static void	userNotRegisteredMsg(AClient &user, std::string cmd){

	if (verboseCheck() >= V_USER)
		std::cout 	<< C_LRED	<< user.getServer()->getName() << " User "
					<< C_RESET	<< user.getBestName() 
					<< C_LRED	" needs to be registered for the "
					<< C_RESET	<< cmd 
					<< C_LRED	" command"
					<< C_RESET	<< std::endl;
}

/* ************************************************************************** *\
*				MSG - User not Operator										   *
\* ************************************************************************** */

static void	userNotOperatorMsg(AClient &user, std::string cmd){

	if (verboseCheck() >= V_USER)
		std::cout 	<< C_LRED	<< user.getServer()->getName() << " User " 
					<< C_RESET	<< user.getBestName() 
					<< C_LRED 	" needs to be operator for the "
					<< C_RESET	<< cmd 
					<< C_LRED 	" command"
					<< C_RESET	<< std::endl;
}

/* ************************************************************************** *\
*				SEND - File Transfer										   *
\* ************************************************************************** */

static void	send(AClient &user, const std::vector<std::string> &args){
	
	if (args.size() != 2){
		user.sendMsg(":" + *user.getServer() + " 461 " + user.getNickName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"File transfer requires 2 parameters : <nick name> of the receiver and <file name> " 
						<<	C_RESET	<<	std::endl;
		return ;
	}

	file_t	sendFile;
	sendFile.senderName = user.getNickName();
	sendFile.receiverName = args[0];
	AClient	*clientName = user.getServer()->getClient(sendFile.receiverName);
	if (clientName == nullptr){
		user.sendMsg(":" + *user.getServer() + " 401 " + sendFile.receiverName + ERR_NOSUCHNICK);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Request rejected  " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" no user can be found for the supplied nickname"
						<<	C_RESET	<<	args[0] <<	std::endl;
		return ;
	}

	sendFile.filePath = args[1];

	size_t	pos = sendFile.filePath.find_last_of('/');
	std::string	fileName = sendFile.filePath.substr(pos + 1);
	
	sendFile.fileName = fileName; 
	std::ifstream inFile(sendFile.filePath, std::ios::binary);
	if (!inFile) {
		user.sendMsg(":" + *user.getServer() + " 402 " + user.getNickName() + " :Invalid file name or path");
		return ;
	}
	std::stringstream buffer;
    buffer << inFile.rdbuf();
	sendFile.line = buffer.str();
	inFile.close();
	
	clientName->sendMsg("User " + user.getNickName() + " is sending you a file called " +fileName);
	clientName->sendMsg("- To accept write [ACCEPT <sender_name> <file_name> <optional: destination_name>]");
	clientName->sendMsg("- To reject write [REJECT <sender_name> <file_name>]");

	user.getServer()->setTransferFile(sendFile.fileName, sendFile);

	user.sendMsg(fileName + " sent");
	if (verboseCheck()	>= V_USER)
		std::cout	<< C_RESET	<< "File "
					<< C_LCYAN	<< fileName
					<< C_RESET	<< " has been sent to user "
					<< C_LCYAN	<< clientName->getNickName()
					<< C_RESET	<< " waiting for reply"
					<< C_RESET	<< std::endl;
}

/* ************************************************************************** *\
*				ACCEPT - File Transfer											*
\* ************************************************************************** */

static void accept(AClient &user, const std::vector<std::string> &args){

	if (args.size() < 2){
		user.sendMsg(":" + *user.getServer() + " 461 " + user.getNickName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"File transfer requires at least 2 parameters : <nick name> of the receiver and <file name> "
						<<	C_RESET	<<	"<nick name>"
						<<	C_LRED	<<	" of the receiver and "
						<<	C_RESET	<<	"<file name>"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	std::string senderName = args[0];

	AClient	*clientName = user.getServer()->getClient(senderName);
	if (clientName == nullptr){
		user.sendMsg(":" + *user.getServer() + " 401 " + senderName + ERR_NOSUCHNICK);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Request rejected  " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" no user can be found for the supplied nickname"
						<<	C_RESET	<<	args[0] <<	std::endl;
		return ;
	}

	std::string argsFile = args[1];

	file_t	sendFile = user.getServer()->getTransferFile(argsFile);
	if (sendFile.fileName.empty()) {
		user.sendMsg(":" + *user.getServer() + " ERROR " + user.getNickName() + " :File not found");
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Error " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" File "
						<<	C_RESET	<<	sendFile.fileName
						<<	C_LRED	<<	" not found"
						<<	C_RESET	<<	std::endl;
		return;
	}

	std::string filePath = argsFile;

	if (args.size() > 2)
		filePath = args[2];

	size_t	pos = filePath.find_last_of('/');
	std::string	newfileName = filePath.substr(pos + 1);
 
	std::fstream outFile(filePath, std::fstream::out);
	if (!outFile.is_open())	{
		user.sendMsg(":" + *user.getServer() + " 402 " + user.getNickName() + " :Error. Failed to open the output file");
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Error " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" File "
						<<	C_RESET	<<	sendFile.fileName
						<<	C_LRED	<<	" failed to open the output file"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	outFile << sendFile.line;
	outFile.close();

	std::string userPath;
	if (args.size() > 2)
		userPath = " as " + filePath;
	user.sendMsg(sendFile.fileName + " accepted" + userPath);
	if (verboseCheck()	>= V_USER)
		std::cout	<< C_RESET	<< "File "
					<< C_LCYAN	<< argsFile
					<< C_RESET	<< " sent by "
					<< C_LCYAN	<< clientName->getNickName()
					<< C_RESET	<< " has been accepted "
					<< C_LCYAN	<< userPath
					<< C_RESET	<< std::endl;
	
	clientName->sendMsg(sendFile.fileName + " accepted");

	// Remove the file entry from the map after processing
	user.getServer()->rmTransferFile(argsFile);
}

/* ************************************************************************** *\
*				REJECT - File Transfer											*
\* ************************************************************************** */

static void reject(AClient &user, const std::vector<std::string> &args) {

	if (args.size() != 2){
		user.sendMsg(":" + *user.getServer() + " 461 " + user.getNickName() + " " + ERR_NEEDMOREPARAMS);
		if (verboseCheck()	>= V_USER)	
			std::cout	<<	C_LRED	<<	"File transfer requires at least 2 parameters : <nick name> of the receiver and <file name> "
						<<	C_RESET	<<	"<nick name>"
						<<	C_LRED	<<	" of the receiver and "
						<<	C_RESET	<<	"<file name>"
						<<	C_RESET	<<	std::endl;
		return ;
	}
	std::string senderName = args[0];
	AClient	*clientName = user.getServer()->getClient(senderName);
	if (clientName == nullptr){
		user.sendMsg(":" + *user.getServer() + " 401 " + senderName + ERR_NOSUCHNICK);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Request rejected  " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" no user can be found for the supplied nickname "
						<<	C_RESET	<<	senderName <<	std::endl;
		return ;

	}
	if (senderName != clientName->getNickName()){
		user.sendMsg(":" + *user.getServer() + " 401 " + senderName + ERR_NOSUCHNICK);
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Request rejected  " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" the nickname "
						<<	C_RESET	<<	senderName
						<<	C_LRED	<<	" is not the sender"
						<<	C_RESET	<<	std::endl;
		return;
	}
	std::string argsFile = args[1];

	file_t	sendFile = user.getServer()->getTransferFile(argsFile);
	if (sendFile.fileName.empty()) {
		user.sendMsg(":" + *user.getServer() + " ERROR " + user.getNickName() + " :File not found");
		if (verboseCheck()	>= V_USER)
			std::cout	<<	C_LRED	<<	"Error " 
						<<	C_RESET	<<	user.getServer()->getName()
						<<	C_LRED	<<	" File "
						<<	C_RESET	<<	sendFile.fileName
						<<	C_LRED	<<	" not found"
						<<	C_RESET	<<	std::endl;
		return;
	}

	user.sendMsg(sendFile.fileName + " rejected");
	clientName->sendMsg(sendFile.fileName + " rejected");

	user.getServer()->rmTransferFile(argsFile);
}
