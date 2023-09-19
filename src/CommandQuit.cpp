/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandQuit.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/31 15:37:09 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/19 14:58:49 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <unistd.h>

void	Command::PrivCommand::quit(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (!args[0].empty())
		user.sendMsg("client " + user.getBestName() + " QUIT :" + args[0]);
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getBestName() 
						<< " QUIT :	" + args[0] << std::endl;
		
	else
		user.sendMsg("client " + user.getBestName() + " QUIT :Leaving.");
		if (verboseCheck() >= V_USER)
			std::cout 	<< "User " << user.getBestName() 
						<< " QUIT :Leaving" << std::endl;
	
	close (user.getPollInfofd());
	user.setPollInfofd(-1); 
	std::cout << "Client " << user.getBestName() << " disconnected from server." << std::endl;
}
