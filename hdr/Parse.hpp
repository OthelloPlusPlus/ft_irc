/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 18:37:36 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/06 17:35:07 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

#include "AClient.hpp"
#include "Command.hpp"
#include <string>

namespace Parse
{
    std::tuple<AClient &, std::string, std::vector<std::string>> \
    parseMsg(AClient &user, std::string msg);
}


#endif