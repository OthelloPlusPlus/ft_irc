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

# include <string>
// std::string
# include <poll.h>
// struct pollfd
# include <netinet/in.h>
// struct sockaddr_in
# include <vector>
// std::vector
# include <map>
// std::map

typedef struct file_s
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
		std::string			serverName;
		struct pollfd		pollInfo;
		struct sockaddr_in	socketAddress;
		int					port;
		std::string			publicIP;	//https://www.whatsmyip.org/	185-61-55-68.185-61-55-ip.fmo-solutions.nl
		std::string			localIP;
		static bool			state;

		std::vector<Channel *>			channels;
		std::vector<AClient *>			clients;
		std::map<std::string, file_t>	fileTr;
		std::string	passwordUser;
		std::string	motd;

		//Server initialisation
		void	validatePort(void) const;
		void	readEnv(void);
		void	setLocalIP(void);
		void	bootUpServer(void);
		void	addBots(void);
		//Server Shutdown
		static void	handlerSIGINT(int signal);
		//Connection subfunctions
		void	acceptClient(void);
		void	sendWelcome(AClient &client) const;

	protected:

	public:
		//(De)constructors
		Server(int argc, char **argv);
		~Server(void);
		//Server Shutdown
		void	shutdownServer(AClient &client, const std::string password);
		//Connection
		void	checkNewClient(void);
		int		validatePassword(const std::string password) const;
		void	checkClients(void);
		void	checkChannels(void);
		//User interaction
		void	sendPrivMsg(AClient &client, const std::vector<std::string> &args) const;
		void	sendNotice(AClient &client, const std::vector<std::string> &args) const;
		void	sendPong(AClient &client, const std::string token) const;
		//Channel Interaction
		void	sendChannelList(AClient &client) const;
		void	joinChannel(AClient &client, const std::vector<std::string> &args);
		void	sendInvite(AClient &client, const std::vector<std::string> &args) const;
		void	partChannel(AClient &client, const std::string channelName);
		void	kickUser(AClient &client, const std::vector<std::string> &args);
		void	sendNames(AClient &client, const std::vector<std::string> &args) const;
		void	sendWho(AClient &client, const std::string who) const;
		void	sendWhoIs(AClient &client, const std::string who) const;
		void	setChannelTopic(AClient &client, const std::vector<std::string> &args);
		void	setChannelMode(AClient &client, const std::vector<std::string> &args);
		//Setters and Getters
		void		setName(void);
		std::string	getName(void) const;
		int			getFD(void) const;
		AClient		*getClient(std::string name) const;
		Channel		*getChannel(std::string channel) const;
		bool		getState(void) const;
		//File Transfer
		void	setTransferFile(std::string key, file_t &file);
		file_t	getTransferFile(std::string key);
		void	rmTransferFile(std::string key);

		//Operator overload to caternate to character strings
# ifdef __APPLE__
		friend std::string operator+(char c, const Server &src)
		{
			return (std::string(1, c) + src);
		}
		friend std::string operator+(const char *str, const Server &src)
		{
			return (std::string(str) + src);
		}
		friend std::string operator+(std::string str, const Server &src)
		{
			if (!src.serverName.empty())
				return (str + src.serverName);
			if (!src.localIP.empty())
				return (str + src.localIP);
			if (!src.publicIP.empty())
				return (str + src.publicIP);
			return (str);
		}

		std::string	operator+(char c)
		{
			return (*this + std::string(1, c));
		}
		std::string	operator+(const char *str)
		{
			return (*this + std::string(str));
		}
		std::string	operator+(std::string str)
		{
			if (!this->serverName.empty())
				return (this->serverName + str);
			if (!this->localIP.empty())
				return (this->localIP + str);
			if (!this->publicIP.empty())
				return (this->publicIP + str);
			return (str);
		}
# else
#  include <type_traits>
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
		std::string	operator+(const T add)
		{
			static_assert(std::is_same<T, char>::value || 
							std::is_same<T, char*>::value || 
							std::is_same<T, const char*>::value ||
							std::is_same<T, std::string>::value, "Invalid type");
			std::string	ret;

			ret = add;
			if (!this->serverName.empty())
				return (this->serverName + ret);
			if (!this->localIP.empty())
				return (this->localIP + ret);
			if (!this->publicIP.empty())
				return (this->publicIP + ret);
			return (ret);
		}
# endif
};

# include "Client.hpp"
# include "AClient.hpp"
# include "Channel.hpp"

#else

class Server;

#endif
