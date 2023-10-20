/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 19:24:50 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/20 20:29:28 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AClient.hpp"

# include <netinet/in.h>
// struct sockaddr_in
// socklen_t
# include <poll.h>
// struct pollfd
# include <iostream>
// std::
# include <string>
// std::string
# include <vector>
// std::vector
# include <tuple>
// for std::tuple
# include <map>
// std::map

class Client : public AClient
{
	private:
		struct sockaddr_in	socketAddress;
		socklen_t			socketAddressLen;
		struct pollfd		pollInfo;
		
		bool				_password;

		void	initialize(int serverFd);
		bool	readReceive(void);
		bool	pollConnection(void);
		bool	hasPassword(void) const;
		void	setPollInfofd(int val);

	protected:	

	public:
		Client(Server &server);
		~Client(void);

		std::string	getMsg(void);
		void		sendMsg(std::string msg);
		void		printInfo(void) const;
		
		std::string	getBestName (void) const; 

		void		passwordValidation(bool val);
		void		setIsRegistered(bool val);


		bool		stillActive(void) const;
		void		closeFD(void);
};

#else

class Client;

#endif
