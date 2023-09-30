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

# include "Client.hpp"
# include "Client.hpp"
# include "Server.hpp"

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
		int			userLimit;
		std::string	key;
		bool		modeTopic;

		Server	*server;
		std::vector<ChannelUser>	users;
		
	protected:

	public:
		Channel(std::string name, Server *server);
		Channel(const Channel &src);
		~Channel(void);

		void	addClient(AClient &newClient, bool admin, const std::string password);
		bool	addClientValidate(const AClient &newClient, const std::string password);

		// void	inviteClient(AClient *client);
		void	removeUser(const AClient &client);
		void	promoteOldestUser(void);

		// void	setAdmin(AClient *target, bool status);
		void	setMode(AClient &client, std::string flag, std::string arg);
		void	setModeI(AClient &client, std::string flag);
		void	setModeT(AClient &client, std::string flag);
		void	setModeK(AClient &client, std::string flag, std::string newPass);
		void	setModeO(AClient &client, std::string flag, std::string targetName);
		void	setModeL(AClient &client, std::string flag, std::string count);
		void	setTopic(AClient &client, const std::string newTopic);

		void	sendTopic(AClient &client) const;
		void	sendNames(AClient &client);
		void	sendWho(AClient &client);
		void	sendToChannel(const std::string msg) const;
		void	sendToChannel(const AClient &exclude, const std::string msg) const;
		// void	sendPrivMsg(AClient *sender, std::string msg);
		void	sendMode(AClient &client) const;

		bool	userIsInChannel(const AClient &client) const;
		ChannelUser	*getChannelUser(std::string clientName);
		bool	userIsAdmin(const AClient &client) const;

		std::string	getName(void) const;
		// std::string	getTopic(void) const;
		size_t		getSize(void) const;
		size_t		getAdminSize(void) const;

		// void	printClientList(void) const;

		Channel	&operator=(const Channel &src);
};

#else

class Channel;

#endif
