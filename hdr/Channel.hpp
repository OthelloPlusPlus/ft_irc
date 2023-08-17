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

	protected:

	public:
		Channel(void);
		Channel(const Channel &src);
		~Channel(void);



		Channel	&operator=(const Channel &src);
};

#endif
