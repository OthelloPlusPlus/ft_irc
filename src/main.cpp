/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/16 15:46:37 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/10/19 15:07:25 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "colors.hpp"

#include <iostream>
// std::
#include <ios>
// std::left
#include <iomanip>
// std::setw()
#include <cstring>
// char *strerror(int errnum);

bool	setEnv(void);

int	main(int argc, char **argv)
{
	std::cout	<< CC_CLEAR	<< std::left
				<< C_HEADER	<< std::setw(76)	<< "ft_irc "
				<< C_RESET	<< "\n";
	if (!setEnv())
		return (false);
	try
	{
		Server	server(argc, argv);

		while (server.getState())
		{
			server.checkNewClient();
			server.checkClients();
			server.checkChannels();
			if (setEnv())
				server.setName();
		}
	}
	catch(const std::range_error &e)
	{
		std::cerr	<< C_RED	<< "Error: "
					<< C_RESET	<< e.what()
					<< std::endl;
	}
	catch(const std::exception& e)
	{
		if (errno == EINTR)
			std::cout	<< C_LCYAN	<< "EINTR(4): "
						<< C_RESET	<< e.what() << strerror(errno)
						<< std::endl;
		else
			std::cerr	<< C_RED	<< "Error: "
						<< C_RESET	<< e.what() << strerror(errno)
						<< std::endl;
	}
	return (errno);
}
