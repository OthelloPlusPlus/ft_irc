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
# include <map>
// std::map

typedef struct file_s		// add this
{
	std::string		fileName;
	std::string		filePath;
	std::string		senderName;
	std::string		receiverName;
	std::string 	line;
}	file_t;

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

		std::map<std::string, file_t>	fileTr; //add this
		
		void	addBots(void);
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

		void	joinChannel(AClient &client, const std::string channelName);
		void	joinChannel(AClient &client, const std::vector<std::string> &args);
		void	partChannel(AClient &client, const std::string channelName);
		void	sendChannelList(AClient &client) const;
		void	sendWho(AClient &client, const std::string who) const;
		void	sendWhoIs(AClient &client, const std::string who) const;
		void	sendPong(AClient &client) const;
		void	sendPong(AClient &client, const std::string token) const;
		void	sendPrivMsg(const AClient &client, const std::vector<std::string> &args);
		void	sendInvite(AClient &client, const std::vector<std::string> &args);
		void	setChannelTopic(AClient &client, const std::vector<std::string> &args);
		void	setChannelMode(AClient &client, const std::vector<std::string> &args);

		Server	&operator=(const Server &src);
		template <typename T>
		friend std::string	operator+(const T add, const Server &src)
		{
			static_assert(std::is_same<T, char>::value || 
							std::is_same<T, char*>::value || 
							std::is_same<T, const char*>::value ||
							std::is_same<T, std::string>::value, "Invalid type");
			std::string	ret;

			ret = add;
			if (!src.serverName.empty())
				return (ret + src.serverName);
			if (!src.localIP.empty())
				return (ret + src.localIP);
			if (!src.publicIP.empty())
				return (ret + src.publicIP);
			return (ret);
		}
		template <typename T>
		std::string operator+(const T add)
		{
			static_assert(std::is_same<T, char>::value || 
							std::is_same<T, char*>::value || 
							std::is_same<T, const char*>::value ||
							std::is_same<T, std::string>::value, "Invalid type");
			std::string	ret;

			ret = add;
			if (!this->serverName.empty())
				return (ret + this->serverName);
			if (!this->localIP.empty())
				return (ret + this->localIP);
			if (!this->publicIP.empty())
				return (ret + this->publicIP);
			return (ret);
		}
};

# include "Client.hpp"
# include "AClient.hpp"
# include "Channel.hpp"

#else

class Server;

#endif
