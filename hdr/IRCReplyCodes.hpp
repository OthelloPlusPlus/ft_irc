/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IRCReplyCodes.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/30 18:02:21 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/08/30 18:02:21 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCREPLYCODES_HPP
# define IRCREPLYCODES_HPP

// https://modern.ircdocs.horse/#numerics

# define RPL_WELCOME				"001"
# define RPL_YOURHOST				"002"
# define RPL_CREATED				"003"
# define RPL_MYINFO					"004"
# define RPL_ISUPPORT				"005"
# define RPL_BOUNCE					"010"
# define RPL_STATSCOMMANDS			"212"
# define RPL_ENDOFSTATS				"219"
# define RPL_STATSUPTIME			"242"
# define RPL_UMODEIS				"221"
# define RPL_LUSERCLIENT			"251"
# define RPL_LUSEROP				"252"
# define RPL_LUSERUNKNOWN			"253"
# define RPL_LUSERCHANNELS			"254"
# define RPL_LUSERME				"255"
# define RPL_ADMINME				"256"
# define RPL_ADMINLOC1				"257"
# define RPL_ADMINLOC2				"258"
# define RPL_ADMINEMAIL				"259"
# define RPL_TRYAGAIN				"263"
# define RPL_LOCALUSERS				"265"
# define RPL_GLOBALUSERS			"266"
# define RPL_WHOISCERTFP			"276"
# define RPL_NONE					"300"
# define RPL_AWAY					"301"
# define RPL_USERHOST				"302"
# define RPL_UNAWAY					"305"
# define RPL_NOWAWAY				" :You have been marked as being away\r\n" //306
# define RPL_WHOREPLY				"352"
# define RPL_ENDOFWHO				"315"
# define RPL_WHOISREGNICK			"307"
# define RPL_WHOISUSER				"311"
# define RPL_WHOISSERVER			"312"
# define RPL_WHOISOPERATOR			"313"
# define RPL_WHOWASUSER				"314"
# define RPL_WHOISIDLE				"317"
# define RPL_ENDOFWHOIS				"318"
# define RPL_WHOISCHANNELS			"319"
# define RPL_WHOISSPECIAL			"320"
# define RPL_LISTSTART				"321"
# define RPL_LIST					"322"
# define RPL_LISTEND				"323"
# define RPL_CHANNELMODEIS			"324"
# define RPL_CREATIONTIME			"329"
# define RPL_WHOISACCOUNT			"330"
# define RPL_NOTOPIC				"331"
# define RPL_TOPIC					"332"
# define RPL_TOPICWHOTIME			"333"
# define RPL_INVITELIST				"336"
# define RPL_ENDOFINVITELIST		"337"
# define RPL_WHOISACTUALLY			"338"
# define RPL_INVITING				"341"
# define RPL_INVEXLIST				"346"
# define RPL_ENDOFINVEXLIST			"347"
# define RPL_EXCEPTLIST				"348"
# define RPL_ENDOFEXCEPTLIST		"349"
# define RPL_VERSION				"351"
# define RPL_NAMREPLY				"353"
# define RPL_ENDOFNAMES				"366"
# define RPL_LINKS					"364"
# define RPL_ENDOFLINKS				"365"
# define RPL_BANLIST				"367"
# define RPL_ENDOFBANLIST			"368"
# define RPL_ENDOFWHOWAS			"369"
# define RPL_INFO					"371"
# define RPL_ENDOFINFO				"374"
# define RPL_MOTDSTART				"375"
# define RPL_MOTD					"372"
# define RPL_ENDOFMOTD				"376"
# define RPL_WHOISHOST				"378"
# define RPL_WHOISMODES				"379"
# define RPL_YOUREOPER				" :You are now an IRC operator\r\n" //381
# define RPL_REHASHING				"382"
# define RPL_TIME					"391"
# define ERR_UNKNOWNERROR			"400"
# define ERR_NOSUCHNICK				"401"
# define ERR_NOSUCHSERVER			" :No such user or server\r\n" // 402
# define ERR_NOSUCHCHANNEL			"403"
# define ERR_CANNOTSENDTOCHAN		"404"
# define ERR_TOOMANYCHANNELS		"405"
# define ERR_WASNOSUCHNICK			"406"
# define ERR_NOORIGIN				" :No origin specified\r\n" 		//409
# define ERR_NORECIPIENT			"411"
# define ERR_NOTEXTTOSEND			"412"
# define ERR_INPUTTOOLONG			"417"
# define ERR_UNKNOWNCOMMAND			" :Unknown command\r\n" 			//421
# define ERR_NOMOTD					"422"
# define ERR_NONICKNAMEGIVEN		" :No nickname given\r\n"		 	//431
# define ERR_ERRONEUSNICKNAME		" :Erroneus nickname\r\n" 			//432
# define ERR_NICKNAMEINUSE			" :Nickname is already in use\r\n" 	//433
# define ERR_NICKCOLLISION			" :Nickname collision KILL from <user>@<host>\r\n" //436
# define ERR_USERNOTINCHANNEL		"441"
# define ERR_NOTONCHANNEL			"442"
# define ERR_USERONCHANNEL			"443"
# define ERR_NOTREGISTERED			"451"
# define ERR_NEEDMOREPARAMS			" :Not enough parameters\r\n" 		//461
# define ERR_ALREADYREGISTERED		" :You may not reregister again\r\n"//462
# define ERR_PASSWDMISMATCH			" :Password incorrect\r\n"			//464
# define ERR_YOUREBANNEDCREEP		"465"
# define ERR_CHANNELISFULL			"471"
# define ERR_UNKNOWNMODE			"472"
# define ERR_INVITEONLYCHAN			"473"
# define ERR_BANNEDFROMCHAN			"474"
# define ERR_BADCHANNELKEY			"475"
# define ERR_BADCHANMASK			"476"
# define ERR_NOPRIVILEGES			" :Permission Denied- You're not an IRC operator\r\n" //481
# define ERR_CHANOPRIVSNEEDED		"482"
# define ERR_CANTKILLSERVER			"483"
# define ERR_NOOPERHOST				" :No O-lines for your host\r\n"	//491
# define ERR_UMODEUNKNOWNFLAG		"501"
# define ERR_USERSDONTMATCH			"502"
# define ERR_HELPNOTFOUND			"524"
# define ERR_INVALIDKEY				"525"
# define RPL_STARTTLS				"670"
# define RPL_WHOISSECURE			"671"
# define ERR_STARTTLS				"691"
# define ERR_INVALIDMODEPARAM		"696"
# define RPL_HELPSTART				"704"
# define RPL_HELPTXT				"705"
# define RPL_ENDOFHELP				"706"
# define ERR_NOPRIVS				"723"
# define RPL_LOGGEDIN				"900"
# define RPL_LOGGEDOUT				"901"
# define ERR_NICKLOCKED				"902"
# define RPL_SASLSUCCESS			"903"
# define ERR_SASLFAIL				"904"
# define ERR_SASLTOOLONG			"905"
# define ERR_SASLABORTED			"906"
# define ERR_SASLALREADY			"907"
# define RPL_SASLMECHS				"908"

#endif
