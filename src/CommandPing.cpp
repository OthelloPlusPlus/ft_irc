/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandPing.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/25 19:10:04 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 14:37:39 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void ping(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.empty() || args[0].empty()){
		std::string serverName = std::getenv("IRC_SERVNAME");
		user.sendMsg(":" + serverName + "461 * " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	server->sendPong(&user, args[0]);
}
