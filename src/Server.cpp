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
#include "Parse.hpp"
#include "IRCReplyCodes.hpp"
#include "verboseCheck.hpp"
#include "BotTicTacToe.hpp"
#include "RockPaperScissors.hpp"

#include <iostream>
// std::cout
#include <iomanip>
// std::setw()
#include <string>
// int	stoi(const char *)
#include <cstring>
// char* strerror( int errnum ); 
#include <unistd.h>
//	int	close(int fildes);
#include <ifaddrs.h>
// int	getifaddrs(ifaddrs **)
// void	freeifaddrs(ifaddrs *)
#include <arpa/inet.h>
// char	*inet_ntoa(in_addr)
#ifdef __APPLE__
# include <fcntl.h>
// int fcntl(int fd, int cmd, ... /* arg */ );
#endif

#include <net/if.h>
// ifaddrs FLAGS
#include <set>
// std::set
#include <chrono>
// std::chrono
#include <algorithm>
// std::transform
#include <tuple>
// for std::tuple

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

Server::Server(int argc, char **argv)
{
	std::cout	<< C_HEADER	<< std::setw(76)	<< "Setting up server "
				<< C_RESET	<< "\n";

	this->pollInfo.fd = -1;
	if (argc < 3)
		throw (std::range_error("Not enough aruments passed."));
	if (argc > 3)
		throw (std::range_error("Too many arguments passed."));
	this->port = std::stoi(argv[1]);
	this->validatePort();
	
	this->passwordUser = argv[2];
	this->readEnv();
	this->setLocalIP();
	this->bootUpServer();
	this->addBots();
	this->motd = "We know what we're doing! We swear!";

	std::cout	<< C_HEADER	<< std::setw(76)	<< "Server setup complete"	<< C_RESET	<< "\n"
				<< std::setw(23)	<< " - Server name: "	<< this->serverName	<< "\n"
				<< std::setw(23)	<< " - Hostname: "	<< this->publicIP	<< "\n"//https://www.whatsmyip.org/	185-61-55-68.185-61-55-ip.fmo-solutions.nl
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
	if (!this->fileTr.empty())
		this->fileTr.clear();
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
	int	reuse = 1;
	if (setsockopt(pollInfo.fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("setsockopt(): "));
	}
#ifdef __APPLE__
	std::cout	<< C_LCYAN	<< "(Apple)"	<< C_RESET	<< "Configuring socket for non-blocking mode...\n";
	if (fcntl(this->pollInfo.fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("fcntl(): "));
	}
#endif
	std::cout	<< "Binding socket to port "	<< this->port	<< "...\n";
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_port = htons(this->port);
	this->socketAddress.sin_addr.s_addr = INADDR_ANY;
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

void	Server::validatePort(void)
{
	std::string msg = "Port " + std::to_string(this->port) + " not allowed. ";
	if (this->port < 0 || this->port > 655535)
		throw (std::range_error((msg + "Outside of port range(1024-655535)").c_str()));
	switch (this->port)
	{
		case 0 ... 1023:	throw (std::range_error((msg + "Well-Known Ports(0-1023) not allowed.").c_str()));
		case 1433:	throw(std::range_error((msg + "Port reserved for Microsoft SQL Server").c_str()));
		case 1521:	throw(std::range_error((msg + "Port reserved for Oracle Database").c_str()));
		case 3306:	throw(std::range_error((msg + "Port reserved for MySQL Database").c_str()));
		case 5000:	throw(std::range_error((msg + "Port reserved for UPnP (Universal Plug and Play) control point").c_str()));
		case 5222:	throw(std::range_error((msg + "Port reserved for XMPP (Extensible Messaging and Presence Protocol)").c_str()));
		case 5432:	throw(std::range_error((msg + "Port reserved for PostgreSQL Database").c_str()));
		case 7070:	throw(std::range_error((msg + "Port reserved for Real-Time Messaging Protocol (RTMP)").c_str()));
		case 8080:	throw(std::range_error((msg + "Port reserved for Alternate HTTP (commonly used for web servers)").c_str()));
		case 8443:	throw(std::range_error((msg + "Port reserved for HTTPS Alternate (often used for secure web traffic)").c_str()));
		case 9090:	throw(std::range_error((msg + "Port reserved for Alternate Web Server (often used for web proxy servers)").c_str()));
		default:	break;
	}
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
		if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_INET && \
			ifap->ifa_flags & IFF_UP && \
			ifap->ifa_flags & IFF_BROADCAST && \
			ifap->ifa_flags & IFF_RUNNING && \
			ifap->ifa_flags & IFF_MULTICAST)
			this->localIP = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin_addr);
	if (this->localIP.empty())
		throw(std::runtime_error("No valid IP found: "));
	this->transferIP = htonl(inet_addr(this->localIP.c_str()));
	freeifaddrs(ifap0);
}

void	Server::readEnv(void)
{
	char	*env;

	std::cout	<< "Reading env for crucial information...\n";
	env = std::getenv("IRCADMINPWD");
	if (!env)
		throw (std::runtime_error("Admin password not found in env."));
	if (this->passwordUser == env)
		throw (std::runtime_error("Invalid password set."));
	env = std::getenv("IRC_SERVNAME");
	if (env)
		this->serverName = env;
	else
		this->serverName = "OMS";
}

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

void	Server::addBots(void)
{
	std::cout	<< std::left
				<< C_HEADER	<< std::setw(76)	<< "Adding bots "
				<< C_RESET	<< "\n";
	for (int i = 0; i >= 0; ++i)
	{
		try
		{
			AClient	*bot = nullptr;
			switch (i)
			{
				case 0:	bot = new BotTicTacToe(*this);	break;
				case 1:	bot = new RockBot(*this);	break;
				default:	i = -2;
			}
			if (!bot)
				break ;
			if (bot && this->getClient(bot->getNickName()) == nullptr)
				this->clients.push_back(bot);
			else
			{
				delete bot;
				throw (std::runtime_error("Bot name already in use"));
			}
		}
		catch(const std::exception& e)
		{
			std::cerr	<< C_RED	<< "Bot Error "
						<< C_RESET	<< i
						<< C_RED	<< ": "
						<< C_RESET	<< e.what()	<< ". Errno: "	<< strerror(errno)
						<< C_RESET	<< std::endl;
		}
		
	}
}
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
	try
	{
		this->clients.push_back(new Client(*this));
	}
	catch(const std::exception& e)
	{
		std::cerr	<< C_RED	<< "Failed to connect Client: "	<< C_RESET
					<< e.what() << std::endl;
	}
}

void	Server::sendWelcome(Client &client)
{
	std::string	msg;

	msg = ':' + *this;
	client.sendMsg(msg + " 375 " + client.getNickName() + " :- ft_irc Message of the Day - ");
	client.sendMsg(msg + " 372 " + client.getNickName() + " :- " + this->motd);
	client.sendMsg(msg + " 376 " + client.getNickName() + " :End of /MOTD command.");
	// this->joinChannel(client, "#WelcomeChannel");
	client.sendMsg(":ServerBot PRIVMSG " + client.getNickName() + " :Hello, I am ServerBot. How can I //help you today?");
}

void	Server::sendChannelList(AClient &client) const
{
	std::string	msg;

	msg = ':' + *this + " 321 " + client.getNickName() + ' ';
	client.sendMsg(msg + "Channel :Users Name");

	msg.replace(msg.find(" 321 "), 5, " 322 ");
	for (std::vector<Channel *>::const_iterator channel = this->channels.begin(); channel != this->channels.end(); ++channel)
		client.sendMsg(msg + **channel + ' ' + std::to_string((*channel)->getSize()) + " :" + (*channel)->getTopic());

	msg.replace(msg.find(" 322 "), 5, " 323 ");
	client.sendMsg(msg + ":END of /LIST");
}

void	Server::sendWho(AClient &client, const std::string who) const
{
	{
		AClient	*whoClient = this->getClient(who);

		if (whoClient != nullptr)
		{
			whoClient->sendMsg(':' + *this + " 352 " + client.getNickName() + "~" + whoClient->getNickName());
			client.sendMsg(':' + *this + " 315 " + client.getNickName() + ' ' + who + " :End of /WHO list");
			return ;
		}
	}
	{
		Channel	*channel = this->getChannel(who);

		if (channel != nullptr)
			channel->sendWho(client);
	}
}

void	Server::sendWhoIs(AClient &client, const std::string who) const
{
	std::string	msg;
	AClient		*whoClient;

	msg = ':' + *this + " 000 " + client.getNickName() + ' ' + who + ' ';
	whoClient = this->getClient(who);
	if (whoClient)
	{
		msg.replace(msg.find(" 000 "), 5, " 311 ");
		client.sendMsg(msg + whoClient->getUserName() + ' ' + whoClient->getClientIP() + " * :" + whoClient->getRealName());
		msg.replace(msg.find(" 311 "), 5, " 312 ");
		client.sendMsg(msg + *whoClient->getServer());
		msg.replace(msg.find(" 312 "), 5, " 301 ");
		if (!whoClient->getAway().empty())
			client.sendMsg(msg + ':' + whoClient->getAway());
		msg.replace(msg.find(" 301 "), 5, " 318 ");
	}
	else
	{
		msg.replace(msg.find(" 000 "), 5, " 401 ");
		client.sendMsg(msg + ":No such nick/channel");
		msg.replace(msg.find(" 401 "), 5, " 318 ");
	}
	client.sendMsg(msg + ":End of /WHOIS list.");
}

void	Server::sendInvite(AClient &client, const std::vector<std::string> &args)
{
	std::set<AClient *>	name;
	std::set<Channel *>	channel;

	for (std::vector<std::string>::const_iterator i = args.begin(); i != args.end(); ++i)
	{
		if ((*i)[0] == '#')
		{
			Channel	*add = this->getChannel(*i);
			if (add != nullptr)
				channel.insert(add);
			else
				client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + *i + " :No such channel");
		}
		else
		{
			AClient	*add = this->getClient(*i);
			if (add != nullptr && add != &client)
				name.insert(add);
			else
				client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + *i + " :No such nick");
		}
	}
	for (std::set<Channel *>::iterator i = channel.begin(); i != channel.end(); ++i)
	{
		for (std::set<AClient *>::iterator j = name.begin(); j != name.end(); ++j)
		{
			if (!(*i)->userIsInChannel(**j))
				(*j)->sendMsg(':' + client + " INVITE " + (*j)->getNickName() + " :" + (*i)->getName());
			else
				client.sendMsg(':' + *this + " 443 " + client.getNickName() + ' ' + (*j)->getNickName() + ' ' + (*i)->getName() + " :is already on channel");
		}
	}
}

// void	Server::sendPong(AClient &client) const
// {
// 	std::string	time;

// 	time = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
// 	client.sendMsg(':' + *this + " PONG " + *this + " :" + time);
// }

void	Server::sendPong(AClient &client, const std::string token) const
{
	client.sendMsg(':' + *this + " PONG " + *this + " :" + token);
}

void	Server::sendPrivMsg(AClient &client, const std::vector<std::string> &args)
{
	std::string	name = args[0];
	std::string msg = args[args.size() - 1];

	if (name.empty())
		return ;
	if (name.at(0) == '#')
	{
		Channel *channel = getChannel(name);

		if (channel != nullptr)
			channel->sendToChannel(client, ':' + client + " PRIVMSG " + *channel + " :" + msg);
		else
			client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + args[0] + " :No such channel");
	}
	else
	{
		AClient	*user = getClient(name);

		if (user != nullptr)
			user->sendMsg(':' + client + " PRIVMSG " + user->getNickName() + " :" + msg);
		else
			client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + args[0] + " :No such client");
	}
}

void	Server::sendNames(AClient &client, const std::vector<std::string> &args)
{
	Channel *channel = this->getChannel(args[0]);

	if (channel != nullptr)
		channel->sendNames(client);
	else
		client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + args[0] + " :No such channel");
}

void	Server::kickUser(AClient &client, const std::vector<std::string> &args)
{
	Channel *channel = this->getChannel(args[0]);

	if (channel != nullptr)
		channel->kickUser(client, args);
	else
		client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + args[0] + " :No such channel");

}

void	Server::sendNotice(AClient &client, const std::vector<std::string> &args)
{
	if (args.size() < 2)
		return ;
	if (args[0][0] == '#')
	{
		Channel	*channel = this->getChannel(args[0]);
		if (channel == nullptr)
			client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + args[0] + " :No such channel");
		else
			channel->sendToChannel(client, ':' + client + " NOTICE " + *channel + " :" + args[1]);
	}
	else
	{
		AClient *target = this->getClient(args[0]);
		if (target == nullptr)
			client.sendMsg(':' + *this + " 401 " + client.getNickName() + ' ' + args[0] + " :No such client");
		else
			target->sendMsg(':' + client + " NOTICE " + *target + " :" + args[1]);
	}
	// uf (args)
	// if (args[0].empty() || args[1].empty())
	// std::cout	<< args.size()	<< std::endl;
	// for (std::vector<std::string>::const_iterator i = args.begin(); i != args.end(); ++i)
	// 	std::cout	<< __func__	<< '\t'	<< *i	<< std::endl;

	// (void)client;
}

// void	Server::sendAuthserv(AClient &client, const std::vector<std::string> &args)
// {
// 	for (std::vector<std::string>::const_iterator i = args.begin(); i != args.end(); ++i)
// 		std::cout	<<	*i	<< std::endl;
// 	client.sendMsg(':' + *(client.getServer()) + " 421 " + client.getNickName() + " AUTHSERV: Feature not supported");
// 	(void)args;
// }

void	Server::checkClients(void)
{
	for (std::vector<AClient *>::iterator client = this->clients.begin(); client != this->clients.end();)
	{
		if ((*client)->stillActive())
		{
			std::string	msg = (*client)->getMsg();
			if (!msg.empty())
			{
				bool	welcome = (*client)->getIsRegistered();
				if (verboseCheck() >= V_MSG)
					std::cout	<< "Recv ["	<< msg.length()	<< "]\t"
								<< C_ORANGE	<< msg
								<< C_RESET	<< std::flush;
				std::tuple<AClient& , std::string, std::vector<std::string>> fwd = Parse::parseMsg(*(dynamic_cast<AClient *>(*client)), msg);
				Command::parseCmd(std::get<0>(fwd), std::get<1>(fwd), std::get<2>(fwd));
				if (welcome == false && (*client)->getIsRegistered())
				// {
					this->sendWelcome(*(dynamic_cast<Client *>(*client)));
				// 	if (verboseCheck() >= V_USER)
				// 		(*client)->printInfo();
				// }
			}
			++client;
		}
		else
		{
			for (std::vector<Channel *>::const_iterator channel = this->channels.begin(); channel != this->channels.end(); ++channel)
				if ((*channel)->userIsInChannel(**client))
					(*channel)->removeUser(**client);
			delete *client;
			client = this->clients.erase(client);
		}
	}
}

int	Server::validatePassword(const std::string password) const
{
	if (std::string(getenv("IRCADMINPWD")) == password)
		return (2);
	return (this->passwordUser == password);
}

void	Server::joinChannel(AClient &client, const std::string channelName)
{
	std::vector<std::string>	channel = {channelName};

	this->joinChannel(client, channel);
}

void	Server::joinChannel(AClient &client, const std::vector<std::string> &args)
{
	Channel		*channel;
	std::string	pass;
	bool		admin;

	if (args.size() <= 0)
		return ;
	channel = this->getChannel(args[0]);
	if (channel == nullptr)
	{
		channel = new Channel(args[0], this);
		this->channels.push_back(channel);
		admin = true;
	}
	else
		admin = client.getIsOperator();
	if (args.size() >= 2)
		pass = args[1];
	channel->addClient(client, admin, pass);
}

void	Server::partChannel(AClient &client, const std::string channelName)
{
	Channel	*channel;

	channel = this->getChannel(channelName);
	if (channel)
		channel->removeUser(client);
}

// std::vector<AClient *>	Server::getClientList(void)
// {
// 	return (this->clients);
// }

AClient	*Server::getClient(std::string name) const
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	for (std::vector<AClient *>::const_iterator client = this->clients.begin(); client != this->clients.end(); ++client)
	{
		std::string	clientName = (*client)->getNickName();

		std::transform(clientName.begin(), clientName.end(), clientName.begin(), ::tolower);
		if (clientName == name)
			return (*client);
	}
	return (nullptr);
}

Channel	*Server::getChannel(std::string name) const
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (name.empty() || name.at(0) != '#')
		name = '#' + name;

	for (std::vector<Channel *>::const_iterator channel = this->channels.begin(); channel != this->channels.end(); ++channel)
	{
		std::string	channelName = (*channel)->getName();

		std::transform(channelName.begin(), channelName.end(), channelName.begin(), ::tolower);
		if (channelName == name)
			return (*channel);
	}
	return (nullptr);
}

std::string	Server::getName(void) const
{
	return (this->serverName);
}

void	Server::setName(void)
{
	char *name = getenv("IRC_SERVNAME");
	if (name != NULL)
		this->serverName = name;
	else
		this->serverName = "OMS";
}

int		Server::getFD(void) const
{
	return (this->pollInfo.fd);
}

// const std::string	Server::getIP(void) const
// {
// 	return (this->localIP);
// }

// uint32_t	Server::getTransferIP(void) const
// {
// 	return (this->transferIP);
// }

void	Server::checkChannels(void)
{
	for (std::vector<Channel *>::iterator channel = this->channels.begin(); channel != this->channels.end();)
	{
		if ((*channel)->getSize() == 0)
		{
			delete (*channel);
			channel = this->channels.erase(channel);
			continue ;
		}
		if ((*channel)->getAdminSize() == 0)
			(*channel)->promoteOldestUser();
		++channel;
	}
}

void	Server::setChannelTopic(AClient &client, const std::vector<std::string> &args)
{
	Channel		*channel = this->getChannel(args[0]);
	std::string	topic = args[args.size() - 1];

	if (channel == nullptr)
		return ;
	channel->setTopic(client, topic);
}

void	Server::setChannelMode(AClient &client, const std::vector<std::string> &args)
{
	Channel		*channel = this->getChannel(args[0]);

	if (channel == nullptr)
	{
		client.sendMsg(':' + *this + " 403 " + client.getNickName() + ' ' + args[0] + " :No such channel");
		return ;
	}
	else if (args.size() == 1)
		channel->sendMode(client);
	else if (args.size() == 2)
		channel->setMode(client, args[1], "");
	else if (args.size() >= 3)
		channel->setMode(client, args[1], args[2]);
}

void	Server::setTransferFile(std::string key, file_t &file)
{
	this->fileTr[key] = file;
}

file_t	Server::getTransferFile(std::string key)
{
	std::map<std::string, file_t>::iterator i = this->fileTr.find(key);
	if (i == this->fileTr.end())
	{
		file_t	newFile;
		return (newFile);
	}
	else
		return (i->second);
}

void	Server::rmTransferFile(std::string key)
{
	this->fileTr.erase(key);
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
	this->passwordUser = src.passwordUser;
	this->localIP = src.localIP;
	return (*this);
}
