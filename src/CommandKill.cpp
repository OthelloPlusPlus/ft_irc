/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandKill.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/19 13:06:54 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/19 15:02:01 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <unistd.h>

void	Command::PrivCommand::kill(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.size() != 2){
		user.sendMsg("461 " + user.getNickName() + " " + cmd + ERR_NEEDMOREPARAMS);
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
			close ((*it)->getPollInfofd());
			(*it)->setPollInfofd(-1); 
			if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << (*it)->getNickName()
						<< " has been killed; reason :"	
						<< args[args.size()] << std::endl;
			break;
			//delete from clients??
		}
		if (it == server->getClientList().end()){
				user.sendMsg("491 * " + (*it)->getNickName() + " " + cmd + ERR_NOOPERHOST);
				return ;
		}
	}
}

/*
The following messages are typically reserved to server operators.
KILL message     Command: KILL   Parameters: <nickname> <comment>
REHASH message   Command: REHASH Parameters: None
RESTART message  Command: RESTART Parameters: None
SQUIT message    Command: SQUIT  Parameters: <server> <comment>
*/
