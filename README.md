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
			<td>[make](Makefile)</td>
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
| [ServerBot](hdr/ServerBot.hpp) | //info | A bot that only works through private messages. He can provide some general server information and shutdown the server. |
| [TicTacBot](hdr/BotTicTacToe.hpp) | //play | A bot that can play Tic Tac Toe. |
| [RockBot](hdr/RockBot.hpp) | //throw | A bot that can play Rock Paper Scissors |

# Explanation
## Enviroment
The project contains an [.env](.env) file which valuable information about the server.
This includes the server name, the password for admins and the level of verbose.
Whenever this file is changes, the server automatically updates accordingly.

## Sockets
Using the [int socket()]() function, a socket is created for internet communication (**AF_INET**) over which can be bidirectional communication (**SOCK_STREAM**) is possible.
Using the [int setsockopt()]() function, the socket is set to bind a socket, even if the address is already in use.
For MacOS systems the function [int fcntl()]() is used to ensure the socket is non-blocking (**O_NONBLOCK**).

## Ports
Then using the [int bind()](https://man7.org/linux/man-pages/man2/bind.2.html) function, the socket is bound to the specified port and the function [int listen()](src/Server.cpp#L211) sets it to listen to the port for incomming streams and maintainting a backlog, which can store 128 (**SOMAXCONN**) incomming messages.
The accepted ports have been limited to avoid usage ports that have predefined uses.

| Port | Usage |
| --- | --- |
| 0 ... 1023 | Well-Known Ports(0-1023) |
| 1433 | Microsoft SQL Server |
| 1521 | Oracle Database |
| 3306 | MySQL Database |
| 5000 | UPnP (Universal Plug and Play) control point |
| 5222 | XMPP (Extensible Messaging and Presence Protocol) |
| 5432 | PostgreSQL Database |
| 7070 | Real-Time Messaging Protocol (RTMP) |
| 8080 | Alternate HTTP (commonly used for web servers) |
| 8443 | HTTPS Alternate (often used for secure web traffic) |
| 9090 | Alternate Web Server (often used for web proxy servers) |

## Classes
The bulk of this program consists of classes. There are classes for the [Server](hdr/Server.hpp), [Channels](hdr/Channel.hpp) and the Clients.
The Clients are derived for the abstract class [AClient](hdr/AClient.hpp), from which the bots are also derived. Once a Bot or Client is added the IRC Server no longer differentiates between them.
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
- https://pidgin.im/
- https://modern.ircdocs.horse/#client-to-server-protocol-structure
