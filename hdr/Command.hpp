/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 14:45:40 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/31 15:18:09 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Client.hpp"
#include "Server.hpp"
#include <string>

//NICK
#define ERR_NONICKNAMEGIVEN		":No nickname given\r\n"						// (431) "<client> :
#define ERR_ERRONEUSNICKNAME	":Erroneus nickname\r\n"						// (432) "<client> <nick> :
#define ERR_NICKNAMEINUSE		":Nickname is already in use\r\n"				// (433) "<client> <nick> :
#define ERR_NICKCOLLISION		":Nickname collision KILL from <user>@<host>\r\n" //(433) "<client> <nick>  :

//PASS
#define ERR_NEEDMOREPARAMS		":Not enough parameters\r\n"					// (461) "<client> <command> :
#define ERR_ALREADYREGISTERED	":You may not reregister\r\n"					// (462) "<client> :
#define ERR_PASSWDMISMATCH		":Password incorrect\r\n"						// (464) "<client> :

//PING : ERR_NEEDMOREPARAMS +
#define ERR_NOORIGIN			":No origin specified\r\n"						// (409) "<client> :

// #define

namespace Command
{

	void cleanMsg(Client &user);
	void parseMsg(Client &user, Server *server);
	void parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server);

	void password(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server);
	void user(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	void nick(Client &user, const std::string &cmd, const std::vector<std::string> &args, std::vector<Client*> clients);
	void ping(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server);
	
	std::vector<std::string> ircSplit( const std::string &input, const std::string &delimiter);
	std::vector<std::string> ircSplitMulti( const std::string &input, const std::string &delimiter);
}

#endif