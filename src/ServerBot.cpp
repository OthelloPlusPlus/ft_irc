
#include "ServerBot.hpp"
#include "verboseCheck.hpp"
#include "Parse.hpp"
#include "colors.hpp"

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

ServerBot::ServerBot(Server &server): AClient(server)
{
	this->_nickName = "ServerBot";
	this->_userName = "ServerBot";
	this->_realName = "Server system management and user help bot";
	if (verboseCheck() >= V_DETAILS)
		std::cout	<< C_GREEN	<< this->_nickName
					<< C_DGREEN	<< ": Server gathers, and now my watch begins."
					<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

ServerBot::~ServerBot(void)
{
	this->closeFD();
	if (verboseCheck() >= V_DETAILS)
		std::cout	<< C_RED	<< this->_nickName
					<< C_DRED	<< ": And now my watch has end."
					<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

bool	ServerBot::stillActive(void) const
{
	return (true);
}

void	ServerBot::sendMsg(std::string msg)
{
	std::cout	<< msg	<< std::endl;
}

std::string	ServerBot::getMsg(void)
{
	return ("");
}

void	ServerBot::closeFD(void)
{}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */
