/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandPass.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/25 15:54:17 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 14:36:56 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <unistd.h>

void password(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server) {
	
	std::string serverName = std::getenv("IRC_SERVNAME");
	if (args.empty() || args[0].empty()){
		user.sendMsg(":" + serverName + "461 " + user.getBestName() + " " + cmd + ERR_NEEDMOREPARAMS);
		if (verboseCheck() >= V_USER)
			std::cout 	<< "Password not submitted, please provide a password"	<< std::endl;
		return ;
	}
	if (user.getIsRegistered() == true){
		user.sendMsg(":" + serverName + "462 " + user.getNickName() + " " + cmd + ERR_ALREADYREGISTERED);
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getNickName() 
						<< " is already registered"	<< std::endl;
		return ;
	}
	
	if (server->validatePassword(args[0]) != 1 && \
		server->validatePassword(args[0]) != 2 ){
		user.sendMsg(":" + serverName + "464 " + user.getBestName() + " " + cmd + ERR_PASSWDMISMATCH);
		close (user.getPollInfofd());
		user.setPollInfofd(-1); 
		if (verboseCheck() >= V_USER)
			std::cout 	<< "Wrong password " 
						<< user.getBestName() 
						<< " disconnected from server"	<< std::endl;
		return ;
	}
	if (server->validatePassword(args[0]) == 2)
		user.setIsOperator(true);
	user.setHasPassword(true);
	user.userRegistration();
}
