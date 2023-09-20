/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandOper.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/01 12:26:31 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 14:37:31 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void	oper(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	
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
		// std::cout << C_RED << (*it)->getNickName() << std::endl;
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

/*current network doesn’t fall within the server’s allowed range.
The following messages are typically reserved to server operators.
Ok - KILL message     Command: KILL   Parameters: <nickname> <comment>
REHASH message   Command: REHASH Parameters: None
RESTART message  Command: RESTART Parameters: None
SQUIT message    Command: SQUIT  Parameters: <server> <comment>
*/
