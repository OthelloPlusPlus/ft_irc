/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/11 13:37:13 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/11 13:39:19 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class AClient;
class Server;

#include <string>
// std::string
#include <vector>
// std::vector

struct ChannelUser
{
	AClient			*client;
	bool			admin;
	unsigned int	timestamp;
};

class Channel
{
	private:
		std::string	name;
		std::string	topic;
		bool		modeInvite;
		size_t		userLimit;
		std::string	key;
		bool		modeTopic;

		Server	*server;
		std::vector<ChannelUser>	users;
		
		bool	addClientValidate(const AClient &newClient, const std::string password);
		bool	userIsAdmin(const AClient &client) const;

		void	sendTopic(AClient &client) const;
		void	sendNames(AClient &client);

		void	setModeI(AClient &client, std::string flag);
		void	setModeT(AClient &client, std::string flag);
		void	setModeK(AClient &client, std::string flag, std::string newPass);
		void	setModeO(AClient &client, std::string flag, std::string targetName);
		void	setModeL(AClient &client, std::string flag, std::string count);

		ChannelUser	*getChannelUser(std::string clientName);

	protected:

	public:
		Channel(std::string name, Server *server);
		Channel(const Channel &src);
		~Channel(void);

		void	addClient(AClient &newClient, bool admin, const std::string password);
		void	removeUser(const AClient &client);
		void	promoteOldestUser(void);
		bool	userIsInChannel(const AClient &client) const;
		// void	inviteClient(AClient *client);

		void	sendWho(AClient &client);
		void	sendToChannel(const std::string msg) const;
		void	sendToChannel(const AClient &exclude, const std::string msg) const;
		// void	sendPrivMsg(AClient *sender, std::string msg);
		void	sendMode(AClient &client) const;

		// void	setAdmin(AClient *target, bool status);
		void	setMode(AClient &client, std::string flag, std::string arg);
		void	setTopic(AClient &client, const std::string newTopic);

		std::string	getName(void) const;
		// std::string	getTopic(void) const;
		size_t		getSize(void) const;
		size_t		getAdminSize(void) const;
		std::string getTopic(void) const;

		// void	printClientList(void) const;

		Channel	&operator=(const Channel &src);
		template <typename T>
		friend std::string	operator+(const T add, const Channel &src)
		{
			static_assert(std::is_same<T, char>::value || 
							std::is_same<T, char*>::value || 
							std::is_same<T, const char*>::value ||
							std::is_same<T, std::string>::value, "Invalid type");
			std::string	ret;

			ret = add;
			if (!src.name.empty())
				return (ret + src.name);
			return (ret);
		}
		template <typename T>
		std::string operator+(const T add)
		{
			static_assert(std::is_same<T, char>::value || 
							std::is_same<T, char*>::value || 
							std::is_same<T, const char*>::value ||
							std::is_same<T, std::string>::value, "Invalid type");
			std::string ret;

			ret = add;
			if (!this->name.empty())
				return (ret + this->name);
			return (ret);
		}
};

# include "AClient.hpp"
# include "Server.hpp"

#else

class Channel;

#endif
