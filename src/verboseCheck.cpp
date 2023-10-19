/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   verboseCheck.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/19 20:25:05 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/10/19 20:25:07 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "colors.hpp"
//  #include <stdlib.h>
// #include <cstdlib>

#include <iostream>
// std::
// std::getenv() setenv()
#include <string.h>
// char *strerror(int errnum);

int	verboseCheck(void)
{
	int verbose;
	try
	{
		verbose = std::atoi(std::getenv("IRC_VERBOSE"));
	}
	catch(const std::exception &e)
	{
		std::cerr	<< C_RED	<< "Error: "	<< C_RESET
					<< e.what() << ' '	<< strerror(errno)
					<< std::endl;
		verbose = -1;
		setenv("IRC_VERBOSE", "-1", 1);
	}
	return (verbose);
}
