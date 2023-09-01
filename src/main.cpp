/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/16 15:46:37 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/09/01 19:08:29 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "colors.hpp"

#include <iostream>
// std::

bool	setEnv(void);

int	main(int argc, char **argv)
{
	if (!setEnv())
		return (false);
	try
	{
		Server	server(argc, argv);

		while (true)
		{
			server.checkNewClient();
			server.checkClients();
			server.checkChannels();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr	<< C_RED	<< "Error: "	<< C_RESET
					<< e.what() << ' '	<< strerror(errno)
					<< std::endl;
		return (errno);
	}
	return (0);
}
