/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 18:37:36 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 18:07:13 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP


#include "Client.hpp"
#include "Command.hpp"
#include <string>

namespace Parse
{
    void parseMsg(Client &user, Server *server);
    // namespace PrivParsing
    // {
    //     void cleanMsg(Client &user);
    //     std::vector<std::string> ircSplit( const std::string &input, const std::string &delimiter);
	// 	std::vector<std::string> ircSplitMulti( const std::string &input, const std::string &delimiter);
    // }
}


#endif