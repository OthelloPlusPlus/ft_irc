/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/16 15:46:37 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/08/16 19:48:49 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <iostream>
// std::

int	main(int argc, char **argv)
{
	try
	{
		Server	server(argc, argv);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << strerror(errno)	<< '\n';
		return (errno);
	}
	// while (true)
	// {

	// }
	return (0);
}
