/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 19:10:04 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/31 15:20:32 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::ping(Client &user, const std::string &cmd, const std::vector<std::string> &args, Server *server){
	if (args.empty() || args[0].empty()){
		user.sendMsg("<client> " + cmd + ERR_NEEDMOREPARAMS);
		return ;
	}
	server->sendPong(&user);
}

//send PONG message with same token args[1]
/*
PING message
     Command: PING
  Parameters: <token>
The PING command is sent by either clients or servers to check the other side of 
the connection is still connected and/or to check for connection latency, at the application layer.

The <token> may be any non-empty string.

When receiving a PING message, clients or servers must reply to it with a PONG message 
with the same <token> value. This allows either to match PONG with the PING they reply to, 
for example to compute latency.

Clients should not send PING during connection registration, though servers may accept it.
Servers may send PING during connection registration and clients must reply to them.

Older versions of the protocol gave specific semantics to the <token> and 
allowed an extra parameter; but these features are not consistently 
implemented and should not be relied on. 
Instead, the <token> should be treated as an opaque value by the receiver.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOORIGIN (409)
*/