/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 14:45:40 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/18 18:42:25 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Client.hpp"
#include <string>

namespace Command
{

	void cleanMsg(Client &user);
	void parseMsg(Client &user);
	void parseCmd(Client &user, const std::string& cmd, const std::string& params);
	
	void user(Client &user, const std::string& cmd, const std::string &params);
	void password(Client &user, const std::string& cmd, const std::string& params);
	void nick(Client &user, const std::string& cmd, const std::string& params);
	
	std::vector<std::string> ircSplit( const std::string& input, const std::string& delimiter);
	std::vector<std::string> ircSplitMulti( const std::string& input, const std::string& delimiter);
}

#endif