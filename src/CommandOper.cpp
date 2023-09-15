/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandOper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 12:26:31 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/14 16:19:07 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void	Command::oper(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.size() != 2){
		user.sendMsg("461 client " + user.getBestName() + " " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
		
	if (server->validatePassword(args[1]) != 2){
		user.sendMsg("464 client " + user.getBestName() + " " + cmd + ERR_PASSWDMISMATCH);
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
			user.sendMsg((*it)->getNickName() + " " + RPL_YOUREOPERM);
			break;
		}
		std::cout << C_RED << (*it)->getNickName() << std::endl;
		if (it == server->getClientList().end()){
				user.sendMsg((*it)->getNickName() + " " + ERR_NOOPERHOSTM);
				return ;
		}
	}
}

/*
Parameters: <name> <password>
check if name is coorect and password is correct else ERR_NOOPERHOST (491)

RPL_YOUREOPER (381)
set as operator, send message 
set MODE and send message

The following messages are typically reserved to server operators.
KILL message     Command: KILL   Parameters: <nickname> <comment>
REHASH message   Command: REHASH Parameters: None
RESTART message  Command: RESTART Parameters: None
SQUIT message    Command: SQUIT  Parameters: <server> <comment>
*/
