/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 16:45:01 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/16 16:45:03 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "User.hpp"

int	main(void)
{
	Server	server;

	if (!server.initialize())
		return (1);
	while (true)
	{
		server.checkNewClients();
		server.checkClients();
	}
	return (0);
}