/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/09 20:45:12 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "colors.hpp"
#include "Command.hpp"
#include "IRCReplyCodes.hpp"

#include <iostream>
// std::
// int	stoi(const char *)
#include <iomanip>
#include <sys/socket.h>
//	int		socket(int domain, int type, int protocol);
//	ssize_t	recv(int socket, void *buffer, size_t length, int flags);
//	int		bind(int socket, const struct sockaddr *address, socklen_t address_len);
//	int		listen(int socket, int backlog);
#include <fcntl.h>
//	int	fcntl(int fildes, int cmd, ...);
#include <unistd.h>
//	int	close(int fildes);
#include <ifaddrs.h>
// int	getifaddrs(ifaddrs **)
// void	freeifaddrs(ifaddrs *)
#include <arpa/inet.h>
// char	*inet_ntoa(in_addr)

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

// Server::Server(void)
// {
// 	std::cout	<< C_DGREEN	<< "Default constructor "
// 				<< C_GREEN	<< "Server"
// 				<< C_DGREEN	<< " called."
// 				<< C_RESET	<< std::endl;
// }
Server::Server(int argc, char **argv)
{
	std::cout	<< std::left
				<< C_HEADER	<< std::setw(76)	<< "Setting up server "
				<< C_RESET	<< "\n";

	this->pollInfo.fd = -1;
	if (argc < 3)
		throw (std::range_error("Not enough aruments passed."));
	if (argc > 3)
		throw (std::range_error("Too many arguments passed."));
	this->port = std::stoi(argv[1]);
	this->password = argv[2];
	this->setLocalIP();
	this->bootUpServer();
	
	std::cout	<< "\n"
				<< C_HEADER	<< std::setw(76)	<< "Server setup complete"	<< C_RESET	<< "\n"
				<< std::setw(23)	<< " - Hostname: "	<< this->publicIP	<< "\n"
				<< std::setw(23)	<< " - Local IP address: "	<< this->localIP	<< "\n"
				<< std::setw(23)	<< " - Port: "	<< this->port	<< "\n"
				<< " - Ready to receive incoming users!"	<< "\n"
				<< C_HEADER	<< std::setw(76)	<< "Server running"	<< C_RESET
				<< std::right	<< std::endl;
}

Server::Server(const Server &src)
{
	*this = src;
	std::cout	<< C_DGREEN	<< "Copy constructor "
				<< C_GREEN	<< "Server"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

Server::~Server(void)
{
	if (this->pollInfo.fd != -1)
		close(this->pollInfo.fd);
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "Server"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

void	Server::bootUpServer(void)
{
	std::cout	<< "Creating socket for incoming connections...\n";
	this->pollInfo.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->pollInfo.fd < 0)
		throw (std::runtime_error("socket(): "));
	std::cout	<< "Configuring socket for non-blocking mode...\n";
	if (fcntl(this->pollInfo.fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("fcntl(): "));
	}
	// if (fcntl(this->pollInfo.fd, F_SETFL, fcntl(this->pollInfo.fd, F_GETFL, 0) | O_NONBLOCK) == -1)
	// {
	// 	close(this->pollInfo.fd);
	// 	throw (std::runtime_error("fcntl(): "));
	// }
	std::cout	<< "Binding socket to port "	<< this->port	<< "...\n";
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_port = htons(this->port);
	this->socketAddress.sin_addr.s_addr = inet_addr(this->localIP.c_str());
	if (bind(this->pollInfo.fd, (struct sockaddr *)&this->socketAddress, sizeof(this->socketAddress)))
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("bind(): "));
	}
	std::cout	<< "Setting socket to listen for incoming connections...\n";
	if (listen(this->pollInfo.fd, SOMAXCONN))
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("listen(): "));
	}
	this->pollInfo.events = POLLIN;
}

void	Server::setLocalIP(void)
{
	char	hostname[4096];
	struct ifaddrs	*ifap0, *ifap;

	if (gethostname(hostname, sizeof(hostname)) == 0)
		this->publicIP = hostname;
	else
		this->publicIP = "";
	if (getifaddrs(&ifap0))
		throw(std::runtime_error("getifaddrs(): "));
	for (ifap = ifap0; ifap != nullptr; ifap = ifap->ifa_next)
		if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_INET)
			this->localIP = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin_addr);
	freeifaddrs(ifap);
}

// void	Server::setPublicIP(void)
// {

// }

// std::string	Server::getHostIp(void)
// {
// 	// std::string		ip;
// 	struct ifaddrs	*ifap0, *ifap;

// 	if (getifaddrs(&ifap0))
// 		throw(std::runtime_error("getifaddrs(): "));
// 	for (ifap = ifap0; ifap != nullptr; ifap = ifap->ifa_next)
// 		if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_INET)
// 			this->localIP = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin_addr);
// 	freeifaddrs(ifap);
// 	return (this->localIP);
// }

void	Server::checkNewClient(void)
{
	if (poll(&this->pollInfo, 1, 0) == -1)
		throw (std::runtime_error("poll(): "));
	if (this->pollInfo.revents == 0)
		return ;
	if (this->pollInfo.revents & POLLIN)
		this->acceptClient();
}

void	Server::acceptClient(void)
{
	Client	*newClient;

	try
	{
		// newClient = new Client;
		// // *newClient = new Client(this->pollInfo.fd);//when magicemy is ready
		// std::cout	<< "Creating new client!"	<< std::endl;
		Client	*newClient = new Client(this->pollInfo.fd);
		// newClient->initialize(this->pollInfo.fd);
		// ipAddressFromSocketAddress(this->socketAddress)
		// newClient->sendMsg(":localhost 375 Othello :- ft_irc Message of the Day - \r\n");
		// newClient->sendMsg(":localhost 372 Othello :- We know what we're doing! We swear!\r\n");
		// newClient->sendMsg(":localhost 376 Othello :End of /MOTD command.\r\n");
		this->clients.push_back(newClient);
		// std::cout	<< "Trying to get info from client"	<< std::endl;
		// newClient->getMsg();
		// if (newClient->getNickName().empty())
		// 	std::cout	<< "no info from client yet!"	<< std::endl;
		// std::cout	<< "Trying to get info from client"	<< std::endl;
		// this->joinChannel(newClient, "#WelcomeChannel");
		// this->joinChannel(newClient, "#WelcomeChannel");
	}
	catch(const std::exception& e)
	{
		std::cerr	<< C_RED	<< "Failed to connect Client: "	<< C_RESET
					<< e.what() << std::endl;
	}
}

#include <chrono>
void	Server::sendWelcome(Client *client)
{
	std::string	msg;

	msg = ":" + this->localIP;
	client->sendMsg(msg + " 375 " + client->getNickName() + " :- ft_irc Message of the Day - \r\n");
	client->sendMsg(msg + " 372 " + client->getNickName() + " :- We know what we're doing! We swear!\r\n");
	client->sendMsg(msg + " 376 " + client->getNickName() + " :End of /MOTD command.\r\n");
	this->joinChannel(client, "#WelcomeChannel");
}

void	Server::sendChannelList(const Client *client) const
{
	std::string	msg;

	msg = ":" + this->publicIP + " 321 " + client->getNickName() + " ";
	client->sendMsg(msg + "Channel :Users  Name \r\n");

	msg.replace(msg.find_first_of(" 321 "), 5, " 322 ");
	for (std::vector<Channel *>::const_iterator channel = this->channels.begin(); channel != this->channels.end(); ++channel)
		client->sendMsg(msg + (*channel)->getName() + " 0 :\r\n");

	msg.replace(msg.find_first_of(" 322 "), 5, " 323 ");
	client->sendMsg(msg + ":END of /LIST\r\n");
}

void	Server::sendPong(const Client *client) const
{
	std::string	time;

	time = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	client->sendMsg(":" + this->localIP + " PONG " + this->localIP + " :" + time + "\r\n");
}


void	Server::checkClients(void)
{
	for (std::vector<Client *>::const_iterator client = this->clients.begin(); client != this->clients.end();)
	{
		if ((*client)->stillActive())
		{
			std::string	msg = (*client)->getMsg();
			if (!msg.empty())
			{
				if ((*client)->getNickName().empty())
				{
					Command::parseMsg(**client, this);
					if (!(*client)->getNickName().empty())
						this->sendWelcome(*client);
					(*client)->printInfo();
				}
				else
					Command::parseMsg(**client, this);
				std::cout	<< "Server received:\n"	
							<< C_ORANGE	<< msg	
							<< C_RESET	<< std::endl;
			}
			++client;
		}
		else
		{
			for (std::vector<Channel *>::const_iterator channel = this->channels.begin(); channel != this->channels.end(); ++channel)
				if ((*channel)->userIsInChannel(*client))
					(*channel)->removeUser(*client);
			delete *client;
			client = this->clients.erase(client);
		}
	}
}

bool	Server::validatePassword(const std::string password) const
{
	return (this->password == password);
}

bool	Server::nicknameExists(const std::string nickname) const
{
	for (std::vector<Client *>::const_iterator i = this->clients.begin(); i != this->clients.end(); ++i)
		if ((*i)->getNickName() == nickname)
			return (true);
	return (false);
}

void	Server::joinChannel(Client *client, const std::string channelName)
{
	for (std::vector<Channel *>::const_iterator i = this->channels.begin(); i != this->channels.end(); ++i)
		if ((*i)->getName() == channelName)
		{
			(*i)->addClient(client);
			return ;
		}
	Channel	*newChannel = new Channel(channelName);
	this->channels.push_back(newChannel);
	newChannel->addClient(client);
}

std::vector<Client *>	Server::getClientList(void)
{
	return (this->clients);
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

Server	&Server::operator=(const Server &src)
{
	if (this == &src)
		return (*this);
	this->pollInfo = src.pollInfo;
	this->socketAddress = src.socketAddress;
	this->clients = src.clients;
	this->port = src.port;
	this->password = src.password;
	this->localIP = src.localIP;
	return (*this);
}
