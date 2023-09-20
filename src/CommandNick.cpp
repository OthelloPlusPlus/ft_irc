/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CommandNick.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/21 15:13:05 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 14:20:10 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void Command::PrivCommand::nick(Client &user, const std::string& cmd, const std::vector<std::string> &params, std::vector<Client*> clients) {
	
	std::string nickname = params[0];

	std::string serverName = getenv("IRC_SERVNAME");
	if (nickname.empty()) {
		user.sendMsg(":" + serverName + " 431 * " + user.getBestName() + " " + ERR_NONICKNAMEGIVEN);
		return ;
	}
	for (std::vector<Client *>::const_iterator i = clients.begin(); i != clients.end(); ++i)
		if ((*i)->getNickName() == nickname && (*i) != &user){
			user.sendMsg("433 * " + nickname + " " +  ERR_NICKNAMEINUSE);
			return ;
		}
	if (isdigit(nickname.at(0))){
		user.sendMsg(":" + serverName + " 432 * " + nickname + " " + ERR_ERRONEUSNICKNAME);
		return ;
	}
	for (int i = 1; i < nickname.size(); i++){
		if (!isalnum(nickname[i])){
			user.sendMsg(":" + serverName + "432 * " + nickname + " " + ERR_ERRONEUSNICKNAME);
			return ;
		}
	}
	user.setNickName(nickname);
	user.userRegistration();
}
