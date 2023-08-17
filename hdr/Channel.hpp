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

# include <netinet/in.h>
// struct sockaddr_in
// socklen_t
# include <poll.h>
// struct pollfd
# include <iostream>
// std::
# include <vector>
// std::vector

class Channel
{
	private:
		std::string	name;
		std::string	topic;
		bool		modeInvite;
		bool		modeTopic;
		std::string	key;
		int			userLimit;

		std::vector<Client *>	operators;
		std::vector<Client *>	clients;

		void	sendTopic(Client *client);
		void	sendNames(Client *client);
		void	sendPrivMsg(Client *sender, std::string msg);

		void	kickClient(Client *client);
		void	inviteClient(Client *client);
		
	protected:

	public:
		Channel(std::string name);
		Channel(const Channel &src);
		~Channel(void);

		void	addClient(Client *newClient);
		// void	sendMsgToChannel(Client *sender, std::string msg);
		void	sendWho(Client *client);

		std::string	getName(void) const;
		std::string	getTopic(void) const;

		Channel	&operator=(const Channel &src);
};

#endif
