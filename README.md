# Intro
> Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
> It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.
> IRC clients connect to IRC servers in order to join channels. IRC servers are connected together to form a network

This program creates and runs an IRC server. It follows the IRC protocols when communicating with IRC clients and has several bots into its programming to interact with.

## Table of Contents
- [Usage](#Usage)
- [Information](#Information)
- [Sources](#Sources)

# Usage
## Running Server
The server has been designed for Linux but should compatible with MacOS too.
It can be compiled and run using the following temrinal commands.

<table>
	<thead>
		<tr><th colspan="3">Terminal Commands</th></tr>
		<tr>
			<th>Command</th>
			<th>Argumens</th>
			<th>Functionality</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td>make</td>
			<td></td>
			<td>Compiles the files into ircserv</td>
		</tr>
		<tr>
			<td>./ircserv</td>
			<td>[port] [password]</td>
			<td>Runs the binary</td>
		</tr>
		<tr>
			<td>make</td>
			<td>subject</td>
			<td>Compiles the files and runs the binary with port '6667' and password set to 'password'</td>
		</tr>
		<tr>
			<td>./nctest.sh</td>
			<td></td>
			<td>Runs a shell script that in turn runs nc. Automatically logs in into the server</td>
		</tr>
	</tbody>
</table>

## Client
While this server, in theory, is compatible with all IRC Clients that follow the IRC protocol. It has been tested using pidgin and is fully compatible with it.

## Bots
While the IRC Server is mainly for client to client communication. It also contains several bots for users to interact with.
| Bots |||
| --- | --- | --- |
| [hdr/ServerBot.hpp](ServerBot) | //info | A bot that only works through private messages. He can provide some general server information and shutdown the server. |
| [hdr/BotTicTacToe.hpp](TicTacBot) | //play | A bot that can play Tic Tac Toe. |
| [hdr/RockBot.hpp](RockBot) | //throw | A bot that can play Rock Paper Scissors |

# Explanation
## Enviroment
The project contains an [.env](.env) file which valuable information about the server.
This includes the server name, the password for admins and the level of verbose.
Whenever this file is changes, the server automatically updates accordingly.
## Ports
We have ports, only good ones allowed!
## Sockets
We have sockets, not to be confused with socks!
## Classes
The bulk of this program consists of classes. There are classes for the [](Server), [](Channels) and the Clients.
The Clients are derived for the abstract class [](AClient), from which the bots are also derived. Once a Bot or Client is added the IRC Server no longer differentiates between them.
The Server has been structured so that new Bots can easily be implemented.
They do require definitions for the following public member functions.

| Return | Public Member Function | Usage |
| --- | --- | --- |
| bool | stillActive(void) const | Whether an AClient is still active |
| void | closeFD(void) | Clean up function for any File Descriptors or alertnatives used |
| std::string | getMsg(void) | Messages the AClient wants to send to the server |
| void | sendMsg(std::string msg) | Messages sent to the AClient |

## Namespsaces
We made space for those too!

# Sources
https://pidgin.im/
https://modern.ircdocs.horse/#client-to-server-protocol-structure
