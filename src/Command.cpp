/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:27:22 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/18 18:50:09 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"
#include <vector>
#include <string>

void Command::parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server){
	if (cmd == "USER")
		Command::PrivCommand::user(user, cmd, args);
	else if (cmd == "NICK")
		Command::PrivCommand::nick(user, cmd, args, server->getClientList());
	else if (cmd == "PASS")
		Command::PrivCommand::password(user, cmd, args, server);
	else if (cmd == "PING")
		Command::PrivCommand::ping(user, cmd, args, server);
	else if (cmd == "QUIT")
		Command::PrivCommand::quit(user, cmd, args, server);
	if (!user.getIsRegistered())
		return;

	if (cmd == "PRIVMSG")
		server->sendPrivMsg(&user, args);
	else if (cmd == "LIST")
		server->sendChannelList(&user);
	else if (cmd == "JOIN")
		server->joinChannel(&user, args[0]);
	else if (cmd == "WHO")
		server->sendWho(&user, args[0]);
	else if (cmd == "WHOIS")
		server->sendWhoIs(&user, args[0]);
	else if (cmd == "PART")
		server->partChannel(&user, args[0]);
	if (!user.getIsOperator())
		return;

	if (cmd == "OPER")
		Command::PrivCommand::oper(user, cmd, args, server);
	else if (cmd == "KILL")
		Command::PrivCommand::kill(user, cmd, args, server);
}
