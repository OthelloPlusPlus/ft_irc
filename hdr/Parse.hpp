/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/24 20:21:57 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/24 20:22:02 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

# include "AClient.hpp"
# include "Command.hpp"
# include <tuple>
// std::tuple
# include <string>
// std::string
# include <vector>
// std::vector

namespace Parse
{
	std::tuple<AClient &, std::string, std::vector<std::string> > \
	parseMsg(AClient &user, std::string msg);
}

#endif
