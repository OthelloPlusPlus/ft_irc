/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 15:54:17 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/13 18:12:21 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <unistd.h>
#include "colors.hpp"

void Command::password(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server) {
	if (args.empty() || args[0].empty()){
		user.sendMsg("461 client " + user.getBestName() + " " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	if (user.getIsRegistered() == true){
		user.sendMsg("462 client " + user.getBestName() + " " + cmd + ERR_ALREADYREGISTERED);
		return ;
	}

	std::string trimmedPass = args[0];
	if (trimmedPass.at(0) == ':')
		trimmedPass.erase(0, 1);
	std::cout << trimmedPass << std::endl;
	std::cout << server->validatePassword(trimmedPass) << std::endl;
	std::cout << user.getIsOperator() << std::endl;
	if (server->validatePassword(trimmedPass) != 1 && \
		server->validatePassword(trimmedPass) != 2 ){
		user.sendMsg("464 client " + user.getBestName() + " " + cmd + ERR_PASSWDMISMATCH);
		close (user.getPollInfofd());
		user.setPollInfofd(-1); 
		std::cout << "Client " << user.getBestName() << " disconnected from server."	<< std::endl;
		return ;
	}
	if (server->validatePassword(trimmedPass) == 2)
		user.setIsOperator(true);
	user.setHasPassword(true);
	user.userRegistration();
}
