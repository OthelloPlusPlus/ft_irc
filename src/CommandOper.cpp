/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandOper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 12:26:31 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/13 15:36:11 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void	Command::oper(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.size() != 2){
		user.sendMsg("461 client " + user.getIpHostName() + " " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
		
	if (server->validatePassword(args[1]) != 2){
		user.sendMsg("464 client " + user.getIpHostName() + " " + cmd + ERR_PASSWDMISMATCH);
		return ;
	}
	if (user.getIsOperator() == false){
		user.sendMsg(user.getNickName() + " :Admin privileges required");
		return ;
	}
	
	std::vector<Client *>::const_iterator it = server->getClientList().begin();
	for (; it != server->getClientList().end(); ++it) {
		Client *u = *it;
		std::cout << C_YELLOW << u->getNickName() << std::endl;
		if (u->getNickName() == args[0] && user.getIsOperator() == true){
			u->setIsOperator(true);
			user.sendMsg(u->getNickName() + " " + RPL_YOUREOPERM); // ??
			break;
		}
		std::cout << C_RED << u->getNickName() << std::endl;
		if (it == server->getClientList().end()){
				user.sendMsg(u->getNickName() + " " + ERR_NOOPERHOSTM);
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



/*
OPER message
Command: OPER
Parameters: <name> <password>

The OPER command is used by a normal user to obtain IRC operator privileges. 
Both parameters are required for the command to be successful.

If the client does not send the correct password for the given name, 
the server replies with an ERR_PASSWDMISMATCH message and the request is not successful.

If the client is not connecting from a valid host for the given name, 
the server replies with an ERR_NOOPERHOST message and the request is not successful.

If the supplied name and password are both correct, and the user is connecting 
from a valid host, the RPL_YOUREOPER message is sent to the user. 
The user will also receive a MODE message indicating their 
new user modes, and other messages may be sent.

The <name> specified by this command is separate to the accounts specified 
by SASL authentication, and is generally stored in the IRCd configuration.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_PASSWDMISMATCH (464)
ERR_NOOPERHOST (491)
RPL_YOUREOPER (381)
Command Example:

  OPER foo bar                ; Attempt to register as an operator
                              using a name of "foo" and the password "bar".
*/