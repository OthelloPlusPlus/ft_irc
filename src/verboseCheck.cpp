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
