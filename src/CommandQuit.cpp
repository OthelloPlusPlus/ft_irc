/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandQuit.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/31 15:37:09 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 13:46:54 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <unistd.h>

void	Command::PrivCommand::quit(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	std::string serverName = getenv("IRC_SERVNAME");
	if (!args[0].empty())
		user.sendMsg(":" + user.getBestName() + "!~" + user.getUserName() + "@" + user.getIpHostName() + " " \
						+ cmd + ":Quit: " + args[0]);
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getBestName() 
						<< " is exiting the network with the message: " + args[0] << std::endl;
	else
		user.sendMsg(":" + user.getBestName() + "!~" + user.getUserName() + "@" + user.getIpHostName() + " " \
						+ cmd + ":quit: ");
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getBestName() 
						<< " is exiting the network" << std::endl;
	
	user.sendMsg("ERROR :Closing Link: " + user.getIpHostName() + " (Client Quit)");
	close (user.getPollInfofd());
	user.setPollInfofd(-1); 
}
