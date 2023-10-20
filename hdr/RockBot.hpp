/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RockBot.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/20 18:18:40 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/20 18:25:24 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROCKBOT_HPP
#define ROCKBOT_HPP

class Server;

# include "AClient.hpp"

# include <string>
// std::string

# include <queue>
//std::queue
# include <map>
//std::map

# define USER 0
# define BOT 1

enum e_move {ROCK, PAPER, SCISSORS};

typedef struct hand_s
{
	int 		moves;
	int			winner[2];
	std::string shapes[2];
	std::string options[3];
}	hand_t;

class RockBot: public AClient
{
	private:
		std::queue<std::string>			recv;
		std::queue<std::string>			send;
		std::map<std::string, hand_t>	hand;
		
		static std::string				funFactsRock[];
		static std::string				funFactsPaper[];
		static std::string				funFactsScissors[];

		void	rockBotRespond(std::string name, const std::string cmd, const std::vector<std::string> &args);
		void	botRespondInvite(const std::vector<std::string> &args);
		void	botRespondJoin(std::string name, const std::vector<std::string> &args);
		void	botRespondPart(const std::vector<std::string> &args);
		void	botRespondPrivMsg(std::string name, const std::vector<std::string> &args);
		void	think(std::string dest, std::string arg);
		
		void	botPlays(std::string dest, std::string arg);
		bool	getPlayerMove(hand_t &hand, std::string arg);
		void	rockMove(std::string dest, hand_t &hand);
		void	botRestartGame(std::string dest);
		void	botShowStats(std::string dest);
		void	helpBot(std::string dest);

		hand_t	findGame(std::string key);
		void	newGame(std::string key);
		void	updateGame(std::string key, hand_t &update);

		// void		funFactsFiller(void);
		std::string	getFunFact(std::string shape);
		
		
	public: 
		RockBot(Server &server);
		~RockBot(void);

		std::string	getMsg(void);
		void		sendMsg(std::string msg);
		bool		stillActive(void) const;
		void		closeFD(void);

};

# include "Server.hpp"

#else

class RockBot;

#endif
