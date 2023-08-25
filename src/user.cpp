/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 19:40:13 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/25 20:48:26 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "colors.hpp"

void Command::user(Client &user, const std::string& cmd, const std::vector<std::string> &args) {
	// size_t space1Pos = params.find(' ', 0);
	// size_t space2Pos = params.find(' ', space1Pos + 1); 
	// size_t colonPos = params.find(':', space2Pos);

	// if (space1Pos != std::string::npos && space2Pos != std::string::npos && colonPos != std::string::npos) {
	// 	std::string idenName = params.substr(0, space1Pos);
	// 	std::string server = params.substr(space2Pos + 1 , colonPos - space2Pos - 2);
	// 	std::string realName = params.substr(colonPos + 1);
		for (int i = 0; i < args.size(); i++) {
			std::cout << "args " << i << " " << args[i] << std::endl;
		}
		
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
}