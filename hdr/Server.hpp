/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/11 13:37:13 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/11 13:39:19 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

class Client;
class AClient;
class Channel;

#include <string>
// std::string
# include <poll.h>
// struct pollfd
# include <netinet/in.h>
// struct sockaddr_in
# include <vector>
// std::vector

class Server
{
	private:
		std::string				serverName;
		struct pollfd			pollInfo;
		struct sockaddr_in		socketAddress;
		int						port;
		std::string				publicIP;
		std::string				localIP;
		uint32_t				transferIP;

		std::vector<Channel *>	channels;
		std::vector<AClient *>	clients;
		std::string				passwordUser;

		// std::string	getHostIp(void);
		void	setLocalIP(void);
		void	bootUpServer(void);
		void	readEnv(void);

		void	acceptClient(void);
		void	sendWelcome(Client &client);

	protected:

	public:
		// Server(void);
		Server(int argc, char **argv);
		Server(const Server &src);
		~Server(void);

		void	checkNewClient(void);
		void	checkClients(void);
		void	checkChannels(void);

		int	validatePassword(const std::string password) const;
		// bool	nicknameExists(const std::string nickname) const;

		std::vector<AClient *>	getClientList(void);
		AClient					*getClient(std::string name) const;
		Channel					*getChannel(std::string channel) const;
		std::string				getName(void) const;
		int						getFD(void) const;
		const std::string		getIP(void) const;
		uint32_t				getTransferIP(void) const;

		void	joinChannel(Client &client, const std::string channelName);
		void	joinChannel(Client &client, const std::vector<std::string> &args);
		void	partChannel(Client &client, const std::string channelName);
		void	sendChannelList(Client &client) const;
		void	sendWho(Client &client, const std::string who) const;
		void	sendWhoIs(Client &client, const std::string who) const;
		void	sendPong(Client &client) const;
		void	sendPong(Client &client, const std::string token) const;
		void	sendPrivMsg(const Client &client, const std::vector<std::string> &args);
		void	sendInvite(Client &client, const std::vector<std::string> &args);
		void	setChannelTopic(Client &client, const std::vector<std::string> &args);
		void	setChannelMode(Client &client, const std::vector<std::string> &args);

		Server	&operator=(const Server &src);
};

# include "Client.hpp"
# include "AClient.hpp"
# include "Channel.hpp"

#else

class Server;

#endif
