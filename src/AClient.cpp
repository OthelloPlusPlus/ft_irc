#include "AClient.hpp"

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

AClient::AClient(std::string serverName): _serverName(serverName), \
										_isRegsitered(false), _isOperator(false)
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


void	AClient::setServerName(std::string serverName)
{
	this->_serverName = serverName;
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
	this->_isRegsitered = value;
}

void	AClient::setIsOperator(bool value)
{
	this->_isOperator = value;
}

void	AClient::setBuffer(std::string buffer)
{
	this->_buffer = buffer;
}

const std::string	&AClient::getServerName(void) const
{
	return (this->_serverName);
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

const bool	&AClient::getIsRegistered(void) const
{
	return (this->_isRegsitered);
}

const bool	&AClient::getIsOperator(void) const
{
	return (this->_isOperator);
}

const std::string	&AClient::getBuffer(void) const
{
	return (this->_buffer);
}
