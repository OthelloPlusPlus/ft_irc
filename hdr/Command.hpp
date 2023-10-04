/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/18 14:45:40 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/04 15:05:26 by emlicame      ########   odam.nl         */
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

enum	e_command
{
	CMD_USER,
	CMD_NICK,
	CMD_PASS, 
	CMD_PING,
	CMD_QUIT,
	CMD_AWAY,
	CMD_SIZE_OPEN,
	
	CMD_PRIVMSG,
	CMD_LIST,
	CMD_JOIN,
	CMD_WHO,
	CMD_WHOIS,
	CMD_PART,
	CMD_INVITE,
	CMD_TOPIC,
	CMD_MODE,
	CMD_SIZE_REGISTERED,

	CMD_OPER,
	CMD_KILL,
	CMD_SIZE_OPER,
	
	CMD_EMPTY,
	CMD_UNKNOWN
};

const int CMD_SIZE_OPEN_INT = CMD_SIZE_OPEN;
const int CMD_SIZE_REGISTERED_INT = CMD_SIZE_REGISTERED;
const int CMD_SIZE_OPER_INT = CMD_SIZE_OPER;

namespace Command
{
	void parseCmd(Client &user, const std::string& cmd, const std::vector<std::string>& args);
	
	static void user(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void password(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void nick(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void ping(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void quit(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void away(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void oper(Client &user, const std::string &cmd, const std::vector<std::string> &args);
	static void kill(Client &user, const std::string &cmd, const std::vector<std::string> &args);

	static void unknownCmd(Client &user, const std::string &cmd);

	static void	userNotRegisteredMsg(Client &user, std::string cmd);
	static void	userNotOperatorMsg(Client &user, std::string cmd);
}

#endif