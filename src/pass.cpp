/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 15:54:17 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/25 21:15:47 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void Command::password(Client &user, const std::string& cmd, const std::vector<std::string>& args, std::vector<Client*>& clients) {
	if (args.empty() || args[0].empty()){
		user.sendMsg("<client> " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	if (args[0].at(0) == ':')
		user.setPassword(args[0].substr(1));
	else
		user.setPassword(args[0]);
}

/*

PASS message
    Command: PASS
  	Parameters: <password>
The PASS command is used to set a ‘connection password’. 

If set, the password must be set before any attempt to register the connection is made. 
This requires that clients send a PASS command before sending the NICK / USER combination.

The password supplied must match the one defined in the server configuration. 
It is possible to send multiple PASS commands before registering but only the last one sent 
is used for verification and it may not be changed once the client has been registered.

If the password supplied does not match the password expected by the server, 
then the server SHOULD send ERR_PASSWDMISMATCH (464) and MAY then close the connection with ERROR. 
Servers MUST send at least one of these two messages.

Servers may also consider requiring SASL authentication upon connection as an alternative to this,
when more information or an alternate form of identity verification is desired.

Numeric replies:

ERR_NEEDMOREPARAMS (461)
ERR_ALREADYREGISTERED (462)
ERR_PASSWDMISMATCH (464)
Command Example:

  PASS secretpasswordhere
*/