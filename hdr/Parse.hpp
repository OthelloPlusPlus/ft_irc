/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 18:37:36 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/21 14:36:21 by emlicame      ########   odam.nl         */
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

}


#endif