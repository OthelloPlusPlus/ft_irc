/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:16:20 by emlicame          #+#    #+#             */
/*   Updated: 2023/08/17 20:23:10 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "Client.hpp"
#include <string>

namespace Message
{
	void cleanMsg(Client &user);
	void parseMsg(Client &user);
}

#endif