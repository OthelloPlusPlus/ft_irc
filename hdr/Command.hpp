/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/18 14:45:40 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/06 17:35:37 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "AClient.hpp"
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
	void parseCmd(AClient &user, const std::string& cmd, const std::vector<std::string>& args);
	
	static void user(AClient &user, const std::string &cmd, const std::vector<std::string> &args);
	static void password(AClient &user, const std::string &cmd, const std::vector<std::string> &args);
	static void nick(AClient &user, const std::string &cmd, const std::vector<std::string> &args);
	static void ping(AClient &user, const std::string &cmd, const std::vector<std::string> &args);
	static void quit(AClient &user, const std::string &cmd, const std::vector<std::string> &args);
	static void away(AClient &user, const std::string &cmd, const std::vector<std::string> &args);
	static void oper(AClient &user, const std::string &cmd, const std::vector<std::string> &args);
	static void kill(AClient &user, const std::string &cmd, const std::vector<std::string> &args);

	static void unknownCmd(AClient &user, const std::string &cmd);

	static void	userNotRegisteredMsg(AClient &user, std::string cmd);
	static void	userNotOperatorMsg(AClient &user, std::string cmd);
}

#endif