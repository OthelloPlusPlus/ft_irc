/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandNick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:13:05 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/13 17:01:49 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void Command::nick(Client &user, const std::string& cmd, const std::vector<std::string> &params, std::vector<Client*> clients) {
		std::string nickname = params[0];

	//	NO NICK name
		if (nickname.empty()) {
			user.sendMsg("431 * client " + user.getBestName() + " " + cmd + ERR_NONICKNAMEGIVEN);
			return ;
		}
	// 	If the nick is already in use ERR_NICKNAMEINUSE (433) and ignore command.
		for (std::vector<Client *>::const_iterator i = clients.begin(); i != clients.end(); ++i)
			if ((*i)->getNickName() == nickname && (*i) != &user){
				user.sendMsg("433 * client " + user.getBestName() + " " + cmd  + ERR_NICKNAMEINUSE);
				return ;
			}
		if (isdigit(nickname[0])){
			user.sendMsg("432 * client " + user.getBestName() + " " + cmd  + ERR_ERRONEUSNICKNAME);
			return ;
		}
		for (int i = 0; i < nickname.size(); i++){
			if (!isalpha(nickname[i])){
				user.sendMsg("432 * client " + user.getBestName() + " " + cmd  + ERR_ERRONEUSNICKNAME);
				return ;
			}
		}
		user.setNickName(nickname);
		user.userRegistration();
}
