/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 19:24:58 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/20 11:36:52 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "colors.hpp"

#include <iostream>

#include <iomanip>
#include <string.h>
// void bzero(void *s, size_t n);
// char *strerror(int errnum);
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

int Client::_verbose = 0;

Client::Client(int serverFD) : _nickName(""), _userName(""), _identName(""), 
								_realName(""), _IpHostName(""), _server(""), _isRegistered(false),
								_isOperator(false), _hasPassword(false) {
	std::cout	<< C_DGREEN	<< "Param constructor "
				<< C_GREEN	<< "Client"
				<< C_DGREEN	<< " called.\n"
				<< C_LMGNT	<< "IRC OMS: To register please use commands PASS - NICK - USER(user_name * host :realname)."
				<< C_RESET	<< std::endl;
	initialize(serverFD);
}

Client::Client(const Client &src) : _nickName(src._nickName), _userName(src._userName),
								_identName(src._identName), _realName(src._realName),
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

void	Client::sendMsg(std::string msg) const {	
	if (verboseCheck() >= V_MSG)
		std::cout	<< "send ["	<< send(this->pollInfo.fd, msg.c_str(), msg.length(), 0)
					<< "]\t"
					<< C_LORANGE	<< msg
					<< C_RESET	<< std::flush;
	else		send(this->pollInfo.fd, msg.c_str(), msg.length(), 0);
}

bool	Client::readReceive(int sockfd){
		char	buffer[4096];
		ssize_t	recvLen;

		bzero(buffer, sizeof(buffer));
		recvLen = recv(this->pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
		if (recvLen < 0) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				return false;	
			} else {
			std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
			return false;
			}
		}
		if (recvLen == 0) {
			close(this->pollInfo.fd);
			this->pollInfo.fd = -1;
			std::cout	<< "Client " << getNickName() << " disconnected from server."	<< std::endl;
			return false;
		}

		this->_buffer.append(buffer);
		return true;

}

std::string	Client::getMsg(void) {
	if (poll(&this->pollInfo, 1, 0) < 0) {
		return "";
	}
	if (this->pollInfo.revents & POLLIN) {
		if(!readReceive(this->pollInfo.fd))
			return "";
	}
	std::string::size_type pos;
	if (!this->_buffer.empty() && (pos = this->_buffer.find("\n")) != std::string::npos)
	{
		// Extract the complete message including the delimiter
		this->_message = this->_buffer.substr(0, pos + 1);
		this->_buffer.erase(0, pos + 1);
		return this->_message;
	}

	return "";
}

bool	Client::stillActive(void) const {
	return (this->pollInfo.fd != -1);
}

void	Client::setVerbose(const int verbose) {
	Client::_verbose = verbose;
}

std::string	const & Client::getBuff( void )const { return _message;}
std::string const & Client::getUserName( void ) const  { return _userName; }
std::string const & Client::getIdentName ( void ) const { return _identName; }
std::string const & Client::getRealName( void ) const  { return _realName; }
std::string const & Client::getNickName( void ) const  { return _nickName; }
std::string const & Client::getServer( void ) const  { return _server; }
std::string const & Client::getIpHostName( void ) const  { return _IpHostName; }
int const & Client::getPollInfofd(void) const { return pollInfo.fd; }
bool Client::getIsRegistered( void ) const  { return _isRegistered; }
bool Client::getIsOperator( void ) const  { return _isOperator; }
bool Client::hasPassword( void ) const  { return _hasPassword; }

std::string	Client::getBestName( void ) const {
	if (!this->_nickName.empty())
		return this->_nickName;
	else if (!this->_IpHostName.empty())
		return _IpHostName;
	return "";
}

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

void Client::setServer(std::string server){
	this->_server = server;
}

void Client::setPollInfofd(int val){
	this->pollInfo.fd = val;
}

void Client::setIpHostName(std::string ipAddress){
	this->_IpHostName = ipAddress;
}

void Client::setIsRegistered(bool val){
	this->_isRegistered = val;
}

void Client::setIsOperator(bool val){
	this->_isOperator = val;
}

void Client::setHasPassword(bool val){
	this->_hasPassword = val;
}

void Client::userRegistration( void ){
	if (hasPassword() == true && !getNickName().empty() && !getIdentName().empty()){
		setIsRegistered(true);
		if (_verbose){
			std::cout	<< std::left 		<< C_HEADER	 
						<< getNickName() 	<< " is now registered in the " << std::getenv("IRC_SERVNAME") //IRC Othello Magic Server" // getEnv("IRC_SERVNAME")
						<< std::setw(76) 	<< C_RESET		<< std::endl;;
		} else {
			std::cout	<< "User " C_CYAN 	<< this->getBestName()
											<< C_RESET " is registered by IRC Othello Magic Server"  << std::endl;
		}
		printInfo();
	}
}


void	Client::printInfo(void) const {
	std::cout << "this->getNickName()\t" << C_BLUE << this->getNickName() << C_RESET	<< std::endl;
	std::cout << "this->getIdentName()\t" << C_BLUE << this->getIdentName() << C_RESET	<< std::endl;
	std::cout << "this->getRealName()\t" << C_BLUE << this->getRealName() << C_RESET	<< std::endl;
	std::cout << "this->getServer()\t" << C_BLUE << this->getServer() << C_RESET	<< std::endl;
	std::cout << "this->getIpHostName()\t" << C_BLUE << this->getIpHostName() << C_RESET	<< std::endl;
	std::cout << "this->getIsRegistered()\t" << C_BLUE << this->getIsRegistered() << C_RESET	<< std::endl;
	std::cout << "this->hasPassword()\t" << C_BLUE << this->hasPassword() << C_RESET	<< std::endl;
	std::cout << "this->getIsOperator\t" << C_BLUE << this->getIsOperator() << C_RESET	<< std::endl;
	std::cout << std::endl;
	// std::cout	<< "socketAddress.sin_addr.s_addr\t"	
	//				<< this->socketAddress.sin_addr.s_addr	<< "\n"
	// 				<< "PollInfo.fd\t"	<< this->pollInfo.fd	<< "\n"
	// 				<< std::flush;
}
// std::cout	<< __func__	<< " " <<  __LINE__	<< std::endl;
// this->sendMsg(":Bot!communicate@localhost NOTICE" + this->getNickName() + " Message received\r\n");


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
	this->_identName = src._identName;
	this->_realName = src._realName;
	this->_IpHostName = src._IpHostName;
	this->_server = src._server;
	this->_isRegistered = src._isRegistered;
	this->_hasPassword = src._hasPassword;
	this->_buffer = src._buffer;
	this->_message = src._message;

	// this->   = src. ; ????

	return (*this);
}

/*

EWOULDBLOCK & EAGAIN)typically used to indicate that a non-blocking socket operation 
would block because there is no data available to read at the moment. 
In other words, these error codes mean that the recv function didn't receive any data 
because the socket is non-blocking and no data was immediately available.

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