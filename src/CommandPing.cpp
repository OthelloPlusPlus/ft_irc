/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 19:10:04 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/31 19:48:23 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::ping(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.empty() || args[0].empty()){
		user.sendMsg("<client> " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	server->sendPong(&user, args[0]);
}
