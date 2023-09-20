/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandUser.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/25 19:40:13 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 11:41:02 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void Command::PrivCommand::user(Client &user, const std::string& cmd, const std::vector<std::string> &args) {
	
	if (user.getIsRegistered()){
		user.sendMsg("462 " + user.getBestName() + " " + cmd + ERR_ALREADYREGISTERED);
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getNickName() 
						<< " is already registered"	<< std::endl;
		return ;
	}

	if (args.size() < 4){
		user.sendMsg("461 " + user.getBestName() + " " + cmd + ERR_NEEDMOREPARAMS);
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_RED << "Error" << C_RESET 
						<< ": format required: USER <user name> * <host> :<realname>" << std::endl;
		return ;
	}

	user.setIdentName(args[0]);
	if (args[1] != "*"){
		if (verboseCheck() >= V_USER)
			std::cerr 	<< C_RED << "Error" << C_RESET 
						<< ": format required: USER <user name> * <host> :<realname>" << std::endl;
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
