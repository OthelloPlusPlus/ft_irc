#include "AClient.hpp"
#include <iostream>

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
	std::cout	<< "this->_clientIP\t"	<< this->_clientIP	<< '\n'
				<< "this->_nickName\t"	<< this->_nickName	<< '\n'
				<< "this->_userName\t"	<< this->_userName	<< '\n'
				<< "this->_realName\t"	<< this->_realName	<< '\n'
				<< "this->_isRegistered\t"	<< this->_isRegistered	<< '\n'
				<< "this->_isOperator\t"	<< this->_isOperator	<< '\n'
				<< "this->_buffer\t"	<< this->_buffer	<< '\n'
				<< std::flush;
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

// std::string	AClient::getSourceName(void) const
// {
// 	return (this->_nickName + "!~" + this->_userName + '@' + this->_clientIP);
// }

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
