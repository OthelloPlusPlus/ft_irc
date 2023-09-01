
#include "colors.hpp"

#include <iostream>
// std::
// std::string
// std::exception
#include <fstream>
// std::ifstream
#include <cstdlib>
// int	setenv(const char *__name, const char *__value, int __overwrite)

static void			readAndSet(std::string file);
static std::string	trimLine(std::string line, std::string trim);

bool	setEnv(void)
{
	try
	{
		readAndSet(".env");
	}
	catch(const std::exception& e)
	{
		std::cerr	<< C_RED	<< "Error: "	<< C_RESET
					<< e.what() << ' '	<< strerror(errno)
					<< std::endl;
		return (false);
	}
	return (true);
}

static void	readAndSet(std::string file)
{
	std::ifstream	fd(file);

	if (!fd.is_open())
		throw (std::runtime_error("Failed to open " + file + "."));

	std::string	line;
	while (std::getline(fd, line))
	{
		size_t	delimPos;

		delimPos = line.find('=');
		if (delimPos == std::string::npos)
		{
			std::cerr	<< "Invalid line in "	<< file	<< ": "	<< line	<< std::endl;
			continue;
		}
		std::string	variable = trimLine(line.substr(0, delimPos), " \t");
		std::string	value = trimLine(line.substr(delimPos + 1), " \t");
		if (setenv(variable.c_str(), value.c_str(), 1))
		{
			fd.close();
			throw (std::runtime_error("setenv(): "));
		}
	}
	fd.close();
}

static std::string	trimLine(std::string line, std::string trim)
{
	if (line.empty() || line.find_first_not_of(trim) == std::string::npos)
		return (line);
	line = line.substr(line.find_first_not_of(trim));
	line = line.substr(0, line.find_last_not_of(trim) + 1);
	return (line);
}
