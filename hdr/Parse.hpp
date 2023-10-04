/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 18:37:36 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/04 14:53:08 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP


#include "Client.hpp"
#include "Command.hpp"
#include <string>

namespace Parse
{
    std::tuple<Client &, std::string, std::vector<std::string>> \
    parseMsg(Client &user, std::string msg);
}


#endif