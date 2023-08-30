/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/09 20:45:12 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "colors.hpp"

#include <iostream>
#include <string>
// std::
#include <unistd.h>
//	int	close(int fildes);
#include <arpa/inet.h>
// char	*inet_ntoa(struct in_addr);

#include "Command.hpp"
// namespace Command

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

//const std::string& ipAddress
Client::Client(int serverFD) : _nickName(""), _userName(""), _password(""), _identName(""), 
								_realName(""), _IpHostName(""), _server(""), _isRegistered(false),
								_hasPassword(false) {
	std::cout	<< C_DGREEN	<< "Param constructor "
				<< C_GREEN	<< "Client"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
	initialize(serverFD);
}

Client::Client(const Client &src) : _nickName(src._nickName), _userName(src._userName),
								_password(src._password), _identName(src._identName), _realName(src._realName),
								_IpHostName(src._IpHostName), _server(src._server), _isRegistered(src._isRegistered),
								_hasPassword(src._hasPassword) {
	*this = src;
	std::cout	<< C_DGREEN	<< "Copy constructor "
				<< C_GREEN	<< "Client"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
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
	setIpHostName(ipAddress(this->socketAddress));
}

void	Client::sendMsg(std::string msg) {
	std::cout	<< "send [" << send(this->pollInfo.fd, msg.c_str(), msg.length(), 0)
				<< "]\t"	<< msg	<< std::endl;
}

std::string	Client::getMsg(void) {
	if (poll(&this->pollInfo, 1, 0) < 0) {
		return "";
	}
	if (this->pollInfo.revents & POLLIN) {	
		char	buffer[10];
		// char	buffer[4096];
		ssize_t	recvLen;

		bzero(buffer, sizeof(buffer));
		recvLen = recv(this->pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
		if (recvLen < 0) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				return "";	
			} else {
			std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
			return "";
			}
		}
		if (recvLen == 0) {
			close(this->pollInfo.fd);
			this->pollInfo.fd = -1;
			std::cout	<< "Client " << getNickName() << " disconnected from server."	<< std::endl;
			return "";
		}

		this->_buffer.append(buffer);
		std::string::size_type pos;

		while ((pos = this->_buffer.find("\n")) != std::string::npos)
		{
			// Extract the complete message including the delimiter
			this->_message = this->_buffer.substr(0, pos + 1);
			this->_buffer.erase(0, pos + 1);
			this->sendMsg(":Bot!communicate@localhost NOTICE Othello Message received\r\n");
			return this->_message;
		}
	}
	return "";
}

/*
EWOULDBLOCK & EAGAIN)typically used to indicate that a non-blocking socket operation 
would block because there is no data available to read at the moment. 
In other words, these error codes mean that the recv function didn't receive any data 
because the socket is non-blocking and no data was immediately available.
*/



bool	Client::stillActive(void) const {
	return (this->pollInfo.fd != -1);
}

std::string	const & Client::getBuff( void )const { return _message;}
std::string const & Client::getUserName( void ) const  { return _userName; }
std::string const & Client::getIdentName ( void ) const { return _identName; }
std::string const & Client::getRealName( void ) const  { return _realName; }
std::string const & Client::getNickName( void ) const  { return _nickName; }
std::string const & Client::getPassword( void ) const  { return _password; }
std::string const & Client::getServer( void ) const  { return _server; }
std::string const & Client::getIpHostName( void ) const  { return _IpHostName; }
bool Client::getIsRegistered( void ) const  { return _isRegistered; }
bool Client::hasPassword( void ) const  { return _hasPassword; }

void Client::setBuff(std::string buffer){
	this->_message = buffer;
}

void Client::setUserName(std::string username){
	this->_userName = username;
}

void Client::setIdentName(std::string identname){
	this->_identName = identname;
}

void Client::setRealName(std::string realname){
	this->_realName = realname;
}

void Client::setNickName(std::string nickname){
	this->_nickName = nickname;
}

void Client::setPassword(std::string password){
	this->_password = password;
}

void Client::setServer(std::string server){
	this->_server = server;
}

void Client::setIpHostName(std::string ipAddress){
	this->_IpHostName = ipAddress;
}

void Client::setIsRegistered(bool val){
	this->_isRegistered = val;
}

void Client::setHasPassword(bool val){
	this->_hasPassword = val;
}

void Client::userRegistration( void ){
	if (hasPassword() == true && !getNickName().empty() && !getIdentName().empty())
		setIsRegistered(true);
}

/*
PASS :Gatto
USER Emanuela_De_La_Vega * 10.11.6.15 :Marylin vos Savant
NICK Magic
*/

void	Client::printInfo(void) const {
	std::cout << "this->getNickName()\t" << C_RED << this->getNickName() << C_RESET	<< std::endl;
	std::cout << "this->getIdentName()\t" << C_RED << this->getIdentName() << C_RESET	<< std::endl;
	std::cout << "this->getRealName()\t" << C_RED << this->getRealName() << C_RESET	<< std::endl;
	std::cout << "this->getPassword()\t" << C_RED << this->getPassword() << C_RESET	<< std::endl;
	std::cout << "this->getServer()\t" << C_RED << this->getServer() << C_RESET	<< std::endl;
	std::cout << "this->getIpHostName()\t" << C_RED << this->getIpHostName() << C_RESET	<< std::endl;
	std::cout << "this->getIsRegistered()\t" << C_RED << this->getIsRegistered() << C_RESET	<< std::endl;
	std::cout << "this->hasPassword()\t" << C_RED << this->hasPassword() << C_RESET	<< std::endl;
	std::cout << std::endl;
	// std::cout	<< "socketAddress.sin_addr.s_addr\t"	
	//				<< this->socketAddress.sin_addr.s_addr	<< "\n"
	// 				<< "PollInfo.fd\t"	<< this->pollInfo.fd	<< "\n"
	// 				<< std::flush;
}
// std::cout	<< __func__	<< " " <<  __LINE__	<< std::endl;

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
	this->_password = src._password;
	this->_identName = src._identName;
	this->_realName = src._realName;
	this->_IpHostName = src._IpHostName;
	this->_server = src._server;
	this->_isRegistered = src._isRegistered;
	this->_hasPassword = src._hasPassword;
	this->_buffer = src._buffer;
	this->_message = src._message;
	// std::vector<std::string> _cmd; ??????
	return (*this);
}

/*
	// if (this->pollInfo.revents & POLLOUT) {
	//		 // The socket is ready for writing. You can send data here if needed.
	//		 // Example: send(this->pollInfo.fd, data_to_send, data_length, 0);
	//		 this->sendMsg(":Bot!communicate@localhost NOTICE Othello POLLOUT Message received\r\n");
	//	 }
	
 	// Check for POLLHUP
	// if (this->pollInfo.revents & POLLHUP) {
	//	 // The remote end has closed the connection.
	//	 close(this->pollInfo.fd);
	//	 this->pollInfo.fd = -1;
	//	 std::cout << "Client " << getNickName() << " disconnected from server." << std::endl;
	// }

	// // Check for POLLERR
	// if (this->pollInfo.revents & POLLERR) {
	//	 // An error condition occurred on the socket.
	//	 std::cerr << "Error on socket: " << strerror(errno) << std::endl;
	//	 close(this->pollInfo.fd);
	//	 this->pollInfo.fd = -1;
	// }

*/