/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 15:54:17 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/31 14:21:22 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <unistd.h>
#include "colors.hpp"

void Command::password(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server) {
	if (args.empty() || args[0].empty()){
		user.sendMsg("<client> " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	if (user.getIsRegistered() == true){
		user.sendMsg("<client> " + cmd + ERR_ALREADYREGISTERED);
		return ;
	}
		
	std::string trimmedPass;
	if (args[0].at(0) == ':')
		trimmedPass = args[0].substr(1);
	else
		trimmedPass = args[0];

	if (!server->validatePassword(trimmedPass) == true){
		user.sendMsg("<client> " + cmd + ERR_PASSWDMISMATCH);
		close (user.getPollInfofd());
		user.setPollInfofd(-1); 
		std::cout << "Client " << user.getNickName() << " disconnected from server."	<< std::endl;
		return ;
	}
	user.setPassword(trimmedPass);
	user.setHasPassword(true);
}
