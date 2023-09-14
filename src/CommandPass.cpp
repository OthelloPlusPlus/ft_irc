/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 15:54:17 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/14 15:16:10 by emlicame         ###   ########.fr       */
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
	
	if (server->validatePassword(args[0]) != 1 && \
		server->validatePassword(args[0]) != 2 ){
		user.sendMsg("464 client " + user.getBestName() + " " + cmd + ERR_PASSWDMISMATCH);
		close (user.getPollInfofd());
		user.setPollInfofd(-1); 
		std::cout << "Client " << user.getBestName() << " disconnected from server."	<< std::endl;
		return ;
	}
	if (server->validatePassword(args[0]) == 2)
		user.setIsOperator(true);
	user.setHasPassword(true);
	user.userRegistration();
}
