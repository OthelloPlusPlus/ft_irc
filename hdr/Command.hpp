/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/18 14:45:40 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/21 14:34:15 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Client.hpp"
#include "Server.hpp"
#include "IRCReplyCodes.hpp"
#include "verboseCheck.hpp"
#include "colors.hpp"
#include <string>

namespace Command
{
	void parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args, Server *server);
	
	static void user(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void password(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server);
	static void nick(Client &user, const std::string &cmd, const std::vector<std::string> &args, std::vector<Client*> clients);
	static void ping(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server);
	static void quit(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server);
	static void oper(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server);
	static void kill(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server);
}

#endif