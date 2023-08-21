/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:13:05 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/21 20:40:36 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "Command.hpp"
// void Command::nick(Client &user, const std::string& cmd, const std::string &params, std::vector<Client*>& clients) {
void Command::nick(Client &user, const std::string& cmd, const std::string &params) {
		std::string nickname = params.substr(0);
		std::cout << "nick " << nickname << "nick size " << nickname.size() << std::endl;
// 		If the nick is already in use ERR_NICKNAMEINUSE (433) and ignore command.
		if (nickname.size() < 1)
			return ;
		for (size_t i = 0; i < clients.size(); ++i) {
        // Do something with clients[i]
			if ((clients[i]->getNickName() == user.getNickName()) && (clients[i] != &user)){
				user.sendMsg(":Nick name already in use"); //ERR_NICKNAMEINUSE 433
			}
   		}
		user.setNickName(nickname);
}
*/
/*
{
			sendMessage(client, "433", cmds[1], ERR_NICKNAMEINUSE);
			return ;
		}
NICK message
     Command: NICK
  Parameters: <nickname>
The NICK command is used to give the client a nickname or change the previous one.

If the server receives a NICK command from a client where the nick is already 
in use on the network, issue an ERR_NICKNAMEINUSE numeric (433) and ignore the NICK command.

If the server does not accept the new nickname supplied by the client as valid (for instance, due to containing invalid characters), it should issue an ERR_ERRONEUSNICKNAME numeric and ignore the NICK command. Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character. Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, including:

no leading # character or other character advertized in CHANTYPES
no leading colon (:)
no ASCII space
If the server does not receive the <nickname> parameter with the NICK command, it should issue an ERR_NONICKNAMEGIVEN numeric and ignore the NICK command.

The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

Numeric Replies:

ERR_NONICKNAMEGIVEN (431)
ERR_ERRONEUSNICKNAME (432)
ERR_NICKNAMEINUSE (433)
ERR_NICKCOLLISION (436)
Command Example:

  NICK Wiz                  ; Requesting the new nick "Wiz".
Message Examples:

  :WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy.

  :dan-!d@localhost NICK Mamoped
                            ; dan- changed his nickname to Mamoped.


void	Server::checkClients(void)
{
	size_t	i;

	i = this->clients.size();
	while (i > 0)
	{
		if (!this->clients[i - 1]->stillActive())
		{
			for (size_t j = this->channels.size(); j > 0; --j)
				if (this->channels[j - 1]->userIsInChannel(this->clients[i - 1]))
					this->channels[j - 1]->removeUser(this->clients[i - 1]);
			delete this->clients[i - 1];
			this->clients.erase(this->clients.begin() + i - 1);
		}
		else
		{
			std::string	msg = this->clients[i - 1]->getMsg();
			if (!msg.empty())
			{
				if (this->clients[i - 1]->getNickName().empty())
				{
					Command::parseMsg(*this->clients[i - 1], this->clients);
					std::cout	<< "Go gentle its my first time..."	<< std::endl;
				}
				else
					Command::parseMsg(*this->clients[i - 1], this->clients);
				std::cout	<< "Server received:\n"	
							<< C_ORANGE	<< msg	
							<< C_RESET	<< std::endl;
				this->clients[i - 1]->printInfo();
				// if (msg == "WHO #WelcomeChannel\r\n")
				// {
				// 	Channel	channel;
				// 	channel.sendWhoToClient(this->clients[i - 1]);
				// }
				// else
				// 	std::cout	<< "diff message!"	<< std::endl;
			}
		}
		--i;
	}
}
*/