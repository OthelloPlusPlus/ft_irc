/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   AClient.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/19 20:24:14 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/10/19 20:24:15 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "AClient.hpp"
#include "verboseCheck.hpp"
#include "colors.hpp"

#include <iostream>
// std::cout

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

AClient::AClient(Server &server): _isOperator(false), _server(server), _isRegistered(false)
{}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

AClient::~AClient(void)
{}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

void	AClient::printInfo(void) const
{
	if (verboseCheck() >= V_DETAILS)
		std::cout	<< C_RESET	<< "this->_clientIP()\t"	<< C_BLUE	<< this->_clientIP	<< '\n'
					<< C_RESET	<< "this->_nickName()\t"	<< C_BLUE	<< this->_nickName	<< '\n'
					<< C_RESET	<< "this->_userName()\t"	<< C_BLUE	<< this->_userName	<< '\n'
					<< C_RESET	<< "this->_realName()\t"	<< C_BLUE	<< this->_realName	<< '\n'
					<< C_RESET	<< "this->_isRegistered()\t"	<< C_BLUE	<< this->_isRegistered	<< '\n'
					<< C_RESET	<< "this->_isOperator()\t"	<< C_BLUE	<< this->_isOperator	<< '\n'
					<< C_RESET	<< std::flush;
}

void	AClient::passwordValidation(bool val)
{
	(void)val;
}

void	AClient::setClientIP(std::string clientIP)
{
	this->_clientIP = clientIP;
}

void	AClient::setNickName(std::string nickName)
{
	this->_nickName = nickName;
}

void	AClient::setUserName(std::string userName)
{
	this->_userName = userName;
}

void	AClient::setRealName(std::string realName)
{
	this->_realName = realName;
}

void	AClient::setAway(std::string away)
{
	this->_away = away;
}

void	AClient::setIsRegistered(bool value)
{
	this->_isRegistered = value;
}

void	AClient::setIsOperator(bool value)
{
	this->_isOperator = value;
}

void	AClient::setBuffer(std::string buffer)
{
	this->_buffer = buffer;
}

Server	*AClient::getServer(void) const
{
	return (&this->_server);
}

const std::string	&AClient::getClientIP(void) const
{
	return (this->_clientIP);
}

const std::string	&AClient::getNickName(void) const
{
	return (this->_nickName);
}

const std::string	&AClient::getUserName(void) const
{
	return (this->_userName);
}

const std::string	&AClient::getRealName(void) const
{
	return (this->_realName);
}

const std::string	&AClient::getBestName(void) const
{
	if (!this->_nickName.empty())
		return (this->_nickName);
	if (!this->_clientIP.empty())
		return (this->_clientIP);
	return (this->_nickName);
}

const std::string	&AClient::getAway(void) const
{
	return (this->_away);
}

const bool	&AClient::getIsRegistered(void) const
{
	return (this->_isRegistered);
}

const bool	&AClient::getIsOperator(void) const
{
	return (this->_isOperator);
}

const std::string	&AClient::getBuffer(void) const
{
	return (this->_buffer);
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

bool	AClient::operator==(const AClient &cmp) const
{
	return (this->_isOperator == cmp._isOperator);
}

bool	AClient::operator!=(const AClient &cmp) const
{
	return (this->_isOperator != cmp._isOperator);
}

bool	AClient::operator>(const AClient &cmp) const
{
	return (this->_isOperator > cmp._isOperator);
}

bool	AClient::operator>=(const AClient &cmp) const
{
	return (this->_isOperator >= cmp._isOperator);
}

bool	AClient::operator<(const AClient &cmp) const
{
	return (this->_isOperator < cmp._isOperator);
}

bool	AClient::operator<=(const AClient &cmp) const
{
	return (this->_isOperator <= cmp._isOperator);
}
