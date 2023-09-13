/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandQuit.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 15:37:09 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/13 17:23:00 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <unistd.h>

void	Command::quit(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (!args[0].empty())
		user.sendMsg("client " + user.getBestName() + " QUIT :" + args[0]);
	else
		user.sendMsg("client " + user.getBestName() + " QUIT :Quit: Leaving.");
	
	close (user.getPollInfofd());
	user.setPollInfofd(-1); 
	std::cout << "Client " << user.getBestName() << " disconnected from server." << std::endl;
}
