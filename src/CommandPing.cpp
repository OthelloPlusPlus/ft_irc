/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 19:10:04 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/18 18:50:55 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::PrivCommand::ping(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.empty() || args[0].empty()){
		user.sendMsg("461 * " + user.getBestName() + " " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	server->sendPong(&user, args[0]);
}
