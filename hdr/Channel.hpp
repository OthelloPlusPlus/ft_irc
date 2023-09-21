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

# ifndef CLIENT_HPP
#  include "Client.hpp"
# else
class Client;
# endif
# ifndef SERVER_HPP
#  include "Server.hpp"
# else
class Server;
# endif

#include <string>
// std::string
#include <vector>
// std::vector

class Channel
{
	struct ChannelUser
	{
		Client			*client;
		bool			admin;
		unsigned int	timestamp;
	};

	private:
		std::string	name;
		std::string	topic;
		bool		modeInvite;
		bool		modeTopic;
		std::string	key;
		int			userLimit;

		Server	*server;
		std::vector<ChannelUser>	users;
		
	protected:

	public:
		Channel(std::string name, Server *server);
		Channel(const Channel &src);
		~Channel(void);

		void	addClient(Client *newClient, bool admin);

		// void	inviteClient(Client *client);
		void	removeUser(const Client *client);
		void	promoteOldestUser(void);

		// void	setAdmin(Client *target, bool status);
		void	setTopic(Client &client, const std::string newTopic);

		void	sendTopic(Client *client) const;
		void	sendNames(Client *client);
		void	sendWho(Client *client);
		void	sendToChannel(const std::string msg) const;
		void	sendToChannel(const Client *exclude, const std::string msg) const;
		// void	sendPrivMsg(Client *sender, std::string msg);

		bool	userIsInChannel(const Client *client) const;

		std::string	getName(void) const;
		// std::string	getTopic(void) const;
		size_t		getSize(void) const;
		size_t		getAdminSize(void) const;

		// void	printClientList(void) const;

		Channel	&operator=(const Channel &src);
};

#endif
