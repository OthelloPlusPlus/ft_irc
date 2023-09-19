/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandOper.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/01 12:26:31 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/19 14:01:03 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void	Command::PrivCommand::oper(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.size() != 2){
		user.sendMsg("461 * " + user.getBestName() + " " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
		
	if (server->validatePassword(args[1]) != 2){
		user.sendMsg("464 * " + user.getBestName() + " " + cmd + ERR_PASSWDMISMATCH);
		return ;
	}
	if (user.getIsOperator() == false){
		user.sendMsg(user.getBestName() + " :Admin privileges required");
		return ;
	}
	
	const std::vector<Client *> &clientList = server->getClientList();

	for (std::vector<Client *>::const_iterator it = clientList.begin(); it != clientList.end(); ++it) {
		std::cout << C_MGNT << (*it)->getNickName() << std::endl;
		if ((*it)->getNickName() == args[0] && user.getIsOperator() == true){
			(*it)->setIsOperator(true);
			user.sendMsg("381 * " + (*it)->getNickName() + " " + RPL_YOUREOPER);
			//set Mode +o
			break;
		}
		// std::cout << C_RED << (*it)->getNickName() << std::endl;
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
