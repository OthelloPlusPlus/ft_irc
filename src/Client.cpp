/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 19:24:58 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/13 18:30:53 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "colors.hpp"
#include "ansicolors.hpp"
#include "Parse.hpp"
// namespace Parse
#include "Command.hpp"
// namespace Command

#include <iomanip>
#include <string.h>
// void bzero(void *s, size_t n);
// char *strerror(int errnum);
#include <unistd.h>
//	int	close(int fildes);
#include <arpa/inet.h>
// char	*inet_ntoa(struct in_addr);
#include <poll.h>
// struct pollfd
#include <iostream>
// std::
#include <vector>
// std::vector
#include <tuple>   
// for std::tuple

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

Client::Client(Server &server) : AClient(server), _password(false) {
	std::cout	<< C_DGREEN	<< "Param constructor "
				<< C_GREEN	<< "Client"
				<< C_DGREEN	<< " called.\n"
				<< C_RESET	<< std::endl;
	initialize(server.getFD());
	this->sendMsg(UHBLU ":" + this->_server + " NOTICE * :*** To register please use commands\n" HBLU "- PASS\n- USER(user_name * host :realname)\n- NICK" CRESET);
}

Client::Client(const Client &src) : AClient(src._server), _password(src._password) {
	*this = src;
	std::cout	<< C_DGREEN	<< "Copy constructor "
				<< C_GREEN	<< "Client"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

/******************************************************************************\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

Client::~Client(void) {
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "Client"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

std::string ipAddress(const struct sockaddr_in& socketAddress) {
	return inet_ntoa(socketAddress.sin_addr);
}

void	Client::initialize(int serverFD) {
	this->socketAddressLen = sizeof(this->socketAddress);
	this->pollInfo.fd = accept(serverFD, (struct sockaddr *)&this->socketAddress, &this->socketAddressLen);
	if (this->pollInfo.fd < 0)
		throw (std::runtime_error("accept(): "));
	this->pollInfo.events = POLLIN;
	this->_clientIP = ipAddress(this->socketAddress);
}

void	Client::sendMsg(std::string msg) {

	if (pollConnection() == false)
		return ;

	msg += "\r\n";
	ssize_t	size = send(this->pollInfo.fd, msg.c_str(), msg.length(), 0);
	if (verboseCheck() >= V_MSG)
		std::cout	<< C_RESET	<< "Send ["	<< size	<< "]\t"
					<< C_LORANGE	<< msg
					<< C_RESET	<< std::flush;
}

bool	Client::readReceive(void){
		char	buffer[4096];
		ssize_t	recvLen;
		bzero(buffer, sizeof(buffer));
		recvLen = recv(this->pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
		std::cout << " buffer [" << buffer << "]" << std::endl;
		if (recvLen < 0) {
			if (errno != EWOULDBLOCK && errno != EAGAIN)
				std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
			return false;
		}
		if (recvLen == 0) {
			this->closeFD();
			std::cout	<< "Client " << getBestName() << " disconnected from server."	<< std::endl;
			return false;
		}
		this->_buffer.append(buffer);
		return true;

}

bool	Client::pollConnection(void)
{
	if (poll(&this->pollInfo, 1, 0) == -1)
		return (false);
	if (this->pollInfo.revents & (POLLERR | POLLHUP))
	{
		close(this->pollInfo.fd);
		this->pollInfo.fd = -1;
		return (false);
	}
	return (true);
}

std::string	Client::getMsg(void) {
	if (pollConnection() == false)
		return "";
	if (this->pollInfo.revents & POLLIN) {
		if(!readReceive())
		{
			return "";
		}
	}
	std::string::size_type pos;
	if (!this->_buffer.empty() && (pos = this->_buffer.find('\n')) != std::string::npos)
	{
		// Extract the complete message including the delimiter
		std::string msg;
		msg = this->_buffer.substr(0, pos + 1);

		this->_buffer.erase(0, pos + 1);

		return msg;
	}

	return "";
}

bool	Client::stillActive(void) const {
	return (this->pollInfo.fd != -1);
}

bool Client::hasPassword(void) const  { return _password; }


std::string	Client::getBestName( void ) const {
	if (!this->_nickName.empty())
		return this->_nickName;
	else if (!this->_clientIP.empty())
		return _clientIP;
	return "";
}

// std::string Client::getSourceName(void) const {
// 	std::stringstream info;
//     info << getBestName() << "!~" << _userName << "@" << _clientIP;
//     return info.str();
// }

void Client::setPollInfofd(int val){
	this->pollInfo.fd = val;
}


void Client::passwordValidation(bool val){
	this->_password = val;
}


void	Client::setIsRegistered(bool val){
	if (hasPassword() == true && !getNickName().empty() && !getUserName().empty()){
		this->_isRegistered = true;
		if (verboseCheck() >= V_USER){
			std::cout	<< std::left 		<< C_HEADER	 
						<< getNickName() 	<< " is now registered in the " << std::getenv("IRC_SERVNAME")
						<< std::setw(76) 	<< C_RESET		<< std::endl;;
		} else {
			std::cout	<< "User " C_CYAN 	<< this->getNickName()
											<< C_RESET " is registered by IRC Othello Magic Server"  << std::endl;
		}
	}
	(void)val;
}

void	Client::printInfo(void) const {

	if (verboseCheck() >= V_DETAILS){
		std::cout << "this->getNickName()\t" << C_BLUE << this->getNickName() << C_RESET	<< std::endl;
		std::cout << "this->getUserName()\t" << C_BLUE << this->getUserName() << C_RESET	<< std::endl;
		std::cout << "this->getRealName()\t" << C_BLUE << this->getRealName() << C_RESET	<< std::endl;
		std::cout << "this->getClientIP()\t" << C_BLUE << this->getClientIP() << C_RESET	<< std::endl;
		std::cout << "this->getIsRegistered()\t" << C_BLUE << this->getIsRegistered() << C_RESET	<< std::endl;
		std::cout << "this->hasPassword()\t" << C_BLUE << this->hasPassword() << C_RESET	<< std::endl;
		std::cout << "this->getIsOperator\t" << C_BLUE << this->getIsOperator() << C_RESET	<< std::endl;
		std::cout << std::endl;
	}	
}

void	Client::closeFD(void)
{
	if (this->pollInfo.fd > 2) {
		close(this->pollInfo.fd);
		this->pollInfo.fd = -1;
	}
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

Client	&Client::operator=(const Client &src) {
	if (this == &src)
		return (*this);
	this->socketAddress = src.socketAddress;
	this->socketAddressLen = src.socketAddressLen;
	this->pollInfo = src.pollInfo;
	this->_nickName = src._nickName;
	this->_userName = src._userName;
	this->_realName = src._realName;
	// this->_IpHostName = src._IpHostName;
	this->_server = src._server;
	this->_isRegistered = src._isRegistered;
	this->_password = src._password;
	this->_buffer = src._buffer;
	// this->_message = src._message;

	// this->   = src. ; ????

	return (*this);
}
