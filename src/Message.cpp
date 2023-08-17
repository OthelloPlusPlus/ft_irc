/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:27:22 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/17 20:22:32 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <vector>
#include <string>

void Message::cleanMsg(Client &user)
{
    std::string del = "\r\n";
    int end = user.getBuff().rfind(del);
    user.setBuff(user.getBuff().substr(0, end)); 
}

void Message::parseMsg(Client &user)
{
	// std::vector<std::string>	cmd;
	// cmd.push_back(message.substr(0, end));
}	

// std::string ircSplit()
// {
	
// }
