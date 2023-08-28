/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 19:40:13 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/28 15:19:06 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void Command::user(Client &user, const std::string& cmd, const std::vector<std::string> &args) {
	// Set the class attributes
	if (args.size() > 3){
		user.setIdentName(args[0]);
		if (args[1] != "*")
			return ;
		user.setServer(args[2]);
		std::string temp;
		if (args[3].at(0) == ':')
			temp = args[3].substr(1);
		for (int i = 3; i < args.size(); i++)
		{
			temp.append(args[i] + " ");
		}
		user.setRealName(temp);
		std::cout << user.getIdentName() << " " <<  user.getServer() << " " <<  user.getRealName() << std::endl;
	}

	if (args.size() > 1)
		user.setIdentName(args[0]);
	if (args.size() > 2 && args[1] != "*")
		return ;
	user.setServer(args[2]);
	std::string temp = args[3].substr(0);
	if (args.size() > 3){
		if (args[3].at(0) == ':')
			temp = args[3].substr(1) + " ";
		for (int i = 4; i < args.size(); i++)
			temp.append(args[i] + " ");
	}
	user.setRealName(temp);
	user.userRegistration();
}