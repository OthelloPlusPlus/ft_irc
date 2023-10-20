/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 18:37:36 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/20 20:32:25 by emlicame      ########   odam.nl         */
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
