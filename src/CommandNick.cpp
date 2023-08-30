/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandNick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:13:05 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/30 14:38:50 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void Command::nick(Client &user, const std::string& cmd, const std::vector<std::string> &params, std::vector<Client*>& clients) {
		std::string nickname = params[0];

		//NO NICK name
		if (nickname.empty()) {
			user.sendMsg("<client> " + user.getNickName() + ERR_NONICKNAMEGIVEN);
			return ;
		}
// 		If the nick is already in use ERR_NICKNAMEINUSE (433) and ignore command.
		for (size_t i = 0; i < clients.size(); ++i) {
			if ((clients[i]->getNickName() == user.getNickName()) && (clients[i] != &user)){
				user.sendMsg("<client> " + user.getNickName() + ERR_NICKNAMEINUSE);
				return ;
			}
		}
		if (isdigit(nickname[0])){
			user.sendMsg("<client> " + user.getNickName() + " :Erroneus nickname\r\n"); //ERR_ERRONEUSNICKNAME (432) 
			return ;
		}
		for (int i = 0; i < nickname.size(); i++){
			if (!isalpha(nickname[i]))
				return ;
		}
			
		user.setNickName(nickname);
		user.userRegistration();
}

/*

NICK message
     Command: NICK
  Parameters: <nickname>
The NICK command is used to give the client a nickname or change the previous one.

- OK	- If the server does not receive the <nickname> parameter with the NICK command, 
			it should issue an ERR_NONICKNAMEGIVEN numeric and ignore the NICK command. ERR_NONICKNAMEGIVEN (431)

- ??OK	- If the server receives a NICK command from a client where the nick is already 
			in use on the network, issue an ERR_NICKNAMEINUSE numeric (433) and ignore the NICK command. ERR_NICKNAMEINUSE (433)

- OK -	If the server does not accept the new nickname supplied by the client as valid 
		(for instance, due to containing invalid characters), it should issue an ERR_ERRONEUSNICKNAME numeric and ignore the NICK command.  ERR_ERRONEUSNICKNAME (432)
		Servers MUST allow at least 
			- all alphanumerical characters, square and curly brackets ([]{}), backslashes (\), and pipe (|) characters in nicknames, 
			- and MAY disallow digits as the first character. 
		Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, including:
			- no leading # character or other character advertized in CHANTYPES
			- no leading colon (:)
			- no ASCII space


The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, 
and to inform other clients about the change of nickname. 
In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

Numeric Replies:

ERR_NICKCOLLISION (436)
Command Example:

  NICK Wiz                  ; Requesting the new nick "Wiz".
Message Examples:

  :WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy.

  :dan-!d@localhost NICK Mamoped
	; dan- changed his nickname to Mamoped.

*/