/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/16 15:46:37 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/09/01 17:34:02 by ohengelm      ########   odam.nl         */
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

// #include <iostream>
// #include <fstream>
// #include <string>
// #include <cstdlib> // for setenv

// void	setEnv(void)
// {
// 	std::ifstream env(".env");
// 	if (!env.is_open())
// 	{
// 		std::cerr	<< "Failed to open .env"	<< std::endl;
// 		return ;
// 	}
// 	std::string	line;
// 	while (std::getline(env, line))
// 	{
// 		size_t	pos;

// 		pos = line.find("=");
// 		if (pos = std::string::npos)
// 			std::cerr	
// 		std::string	variable = line.substr(line.find_first_not_of(" \t"), line.find_last_not_of(" \t", line.find("=")));;
// 		std::string	value = line.substr(line.find_first_not_of(" \t", line.find("=") + 1));
// 		std::cout	<< variable	<< '\n'
// 					<< value	<< std::endl;
// 		// size_t	pos = line.find('=');
// 		// std::cout	<< "setting line: "	<< line	<< std::endl;
// 		// if (pos != std::string::npos)
// 		// 	if (setenv(line.substr(0, pos).c_str(), line.substr(pos + 1).c_str(), 1))
// 		// 		std::cerr	<< "Failed to set env:"	<< line	<< std::endl;;
// 	}
// 	env.close();
// }
