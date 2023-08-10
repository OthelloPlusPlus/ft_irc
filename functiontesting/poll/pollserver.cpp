#include <poll.h>
//	int	poll(struct pollfd fds[], nfds_t nfds, int timeout);
#include <iostream>
//	std::cout
#include <sys/socket.h>
//	ssize_t	recv(int socket, void *buffer, size_t length, int flags);
//	int	bind(int socket, const struct sockaddr *address, socklen_t address_len);
//	int	listen(int socket, int backlog);
#include <unistd.h>
//	int	close(int fildes);
#include <sys/socket.h>
//	int	socket(int domain, int type, int protocol);
#include <netinet/in.h>
//	struct sockaddr_in
#include <arpa/inet.h>
//	in_addr_t	inet_addr(const char *cp);
//	uint16_t	htons(uint16_t hostshort);
//	struct sockaddr_in
#include <fcntl.h>
//	int	fcntl(int fildes, int cmd, ...);

typedef struct	s_connect
{
	int					socketFd;
	struct sockaddr_in	socketAddress;
	socklen_t			socketAddressLen;
	struct pollfd		pollInfo;
}	t_connect;

bool	setServer(t_connect &server);
bool	checkRevents(short &revents);
void	acceptClient(t_connect &client, t_connect &server);
void	sendToClient(int socket, std::string message);

int	main(void)
{
	t_connect		server;
	t_connect		client;
	struct pollfd	pollInfo;

	if (!setServer(server))
		return (true);
	while (true)
	{
		if (poll(&server.pollInfo, 1, 0) < 0)
		{
			std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
			break ;
		}
		if (server.pollInfo.revents & POLLIN)
			acceptClient(client, server);
		if (poll(&client.pollInfo, 1, 0) < 0)
		{
			std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
			break ;
		}
		if (client.pollInfo.revents & POLLIN)
		{
			char	buffer[4096];
			ssize_t	readLen;

			readLen = recv(client.pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
			if (readLen < 0)
				std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
			if (readLen == 0)
			{
				close(client.pollInfo.fd);
				client.pollInfo.fd = 0;
				std::cout	<< "Client disconnected from server."	<< std::endl;
			}
			else
			{
				std::cout	<< "message received: "	<< buffer << std::endl;
				sendToClient(client.socketFd, ":localhost 001 Othello :Welcome to the Libera.Chat Internet Relay Chat Network Othello\r\n");
				sendToClient(client.socketFd, ":localhost 002 Othello :Your host is localhost[127.0.0.1/6667], running version solanum-1.0-dev\r\n");
				sendToClient(client.socketFd, ":localhost 003 Othello :This server was created Thu Jun 15 2023 at 07:38:08 UTC\r\n");
				sendToClient(client.socketFd, ":localhost 004 Othello localhost solanum-1.0-dev DGMQRSZaghilopsuwz CFILMPQRSTbcefgijklmnopqrstuvz bkloveqjfI\r\n");
				sendToClient(client.socketFd, ":localhost 005 Othello ETRACE SAFELIST ELIST=CMNTU MONITOR=100 CALLERID=g FNC WHOX KNOCK CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQRSTcgimnprstuz :are supported by this server\r\n");
				sendToClient(client.socketFd, ":localhost 005 Othello CHANLIMIT=#:250 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=Libera.Chat STATUSMSG=@+ CASEMAPPING=rfc1459 NICKLEN=16 MAXNICKLEN=16 CHANNELLEN=50 TOPICLEN=390 DEAF=D :are supported by this server\r\n");
				sendToClient(client.socketFd, ":localhost 005 Othello TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: EXTBAN=$,agjrxz :are supported by this server\r\n");
				sendToClient(client.socketFd, ":localhost 252 Othello 40 :IRC Operators online\r\n");
				sendToClient(client.socketFd, ":localhost 253 Othello 80 :unknown connection(s)\r\n");
				sendToClient(client.socketFd, ":localhost 254 Othello 22370 :channels formed\r\n");
				sendToClient(client.socketFd, ":localhost 255 Othello :I have 1829 clients and 1 servers\r\n");
				sendToClient(client.socketFd, ":localhost 265 Othello 1829 3634 :Current local users 1829, max 3634\r\n");
				sendToClient(client.socketFd, ":localhost 266 Othello 31655 45716 :Current global users 31655, max 45716\r\n");
				sendToClient(client.socketFd, ":localhost 250 Othello :Highest connection count: 3635 (3634 clients) (97929 connections received)\r\n");
				sendToClient(client.socketFd, ":localhost 375 Othello :- calcium.libera.chat Message of the Day - \r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- Welcome to Libera Chat, the IRC network for \r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- free & open-source software and peer directed projects.\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :-  \r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- Use of Libera Chat is governed by our network policies.\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :-  \r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- To reduce network abuses we perform open proxy checks\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- on hosts at connection time.\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :-  \r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- Please visit us in #libera for questions and support.\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :-  \r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- Website and documentation:  https://libera.chat\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- Webchat:                    https://web.libera.chat\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- Network policies:           https://libera.chat/policies\r\n");
				sendToClient(client.socketFd, ":localhost 372 Othello :- Email:                      support@libera.chat\r\n");
				sendToClient(client.socketFd, ":localhost 376 Othello :End of /MOTD command.\r\n");
				sendToClient(client.socketFd, ":Othello MODE Othello :+iw\r\n");
				// sendToClient(client.socketFd, ":NickServ!NickServ@127.0.0.1 NOTICE Othello :Othello is not a registered nickname.\r\n");
				sendToClient(client.socketFd, ":Othello!~Othello JOIN #TempTest\r\n");
				// sendToClient(client.socketFd, "\r\n");
				
				// sendToClient(client.socketFd, "PING :localhost");
			}
		}
	}
}

void	sendToClient(int socket, std::string message)
{
	std::cout	<< "send() "
				<< send(socket, message.c_str(), message.length(), 0)
				<< std::endl;
}

bool	setServer(t_connect &server)
{
	server.socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (server.socketFd < 0)
	{
		std::cerr	<< "Error socket(): "	<< strerror(errno)	<< std::endl;
		return (false);
	}
	if (fcntl(server.socketFd,  F_SETFL, fcntl(server.socketFd, F_GETFL, 0) | O_NONBLOCK) == -1)
	{
		std::cerr	<< "Error fctnl(): "	<< strerror(errno)	<< std::endl;
		close(server.socketFd);
		return (false);
	}
	server.socketAddress.sin_family = AF_INET;
	server.socketAddress.sin_port = htons(6667);
	server.socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(server.socketFd, (struct sockaddr *)&server.socketAddress, sizeof(server.socketAddress)) == -1)
	{
		std::cerr	<< "Error bind(): "	<< strerror(errno)	<< std::endl;
		close(server.socketFd);
		return (false);
	}
	if (listen(server.socketFd, SOMAXCONN) == -1)
	{
		std::cerr	<< "Error listen(): "	<< strerror(errno)	<< std::endl;
		close(server.socketFd);
		return (false);
	}
	server.pollInfo.fd = server.socketFd;
	server.pollInfo.events = POLLIN;
	return (true);
}

void	acceptClient(t_connect &client, t_connect &server)
{
	client.socketAddressLen = sizeof(client.socketAddress);
	client.socketFd = accept(server.socketFd, (struct sockaddr *)&client.socketAddress, &client.socketAddressLen);
	if (client.socketFd == -1)
		std::cerr	<< "Error accept(): "	<< strerror(errno)	<< std::endl;
	client.pollInfo.events = POLLIN;
	client.pollInfo.fd = client.socketFd;
	std::cout	<< "Connection accepted"	<< std::endl;
}

// bool	checkRevents(short &revents)
// {
// 	if (revents & POLLNVAL)
// 		std::cout	<< "requested events \"invalid\""	<< '\n';
// 	// if (revents & POLLHUP)
// 	// 	std::cout	<< "file descriptor was \"hung up\""	<< '\n';
// 	if (revents & POLLERR)
// 		std::cout	<< "some poll error occurred"	<< '\n';
// 	if (revents & POLLOUT)
// 		std::cout	<< "file descriptor is writeable"	<< '\n';
// 	if (revents & POLLPRI)
// 		std::cout	<< "OOB/Urgent readable data"	<< '\n';
// 	// if (revents & POLLIN)
// 	// 	std::cout	<< "any readable data available"	<< '\n';
// 	std::cout	<< std::flush;
// 	return (revents == POLLERR);
// }





// #include <iostream>
// #include <vector>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <poll.h>
// #include <unistd.h>

// int main() {
//     // Create server socket
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         std::cerr << "Error creating server socket" << std::endl;
//         return 1;
//     }

//     // Set server address
//     struct sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(12345);

//     // Bind server socket to the address
//     if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
//         std::cerr << "Error binding server socket" << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(serverSocket, SOMAXCONN) == -1) {
//         std::cerr << "Error listening on server socket" << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     // Create a vector to store pollfd structures
//     std::vector<struct pollfd> pollFds;

//     // Add the server socket to the pollfd array
//     struct pollfd serverPollInfo;
//     serverPollInfo.fd = serverSocket;
//     serverPollInfo.events = POLLIN;
//     pollFds.push_back(serverPollInfo);

//     std::cout << "Server is running and listening for incoming connections..." << std::endl;

//     while (true) {
//         int ready = poll(pollFds.data(), pollFds.size(), -1);
//         if (ready == -1) {
//             std::cerr << "Error in poll: " << strerror(errno) << std::endl;
//             break;
//         }

//         for (size_t i = 0; i < pollFds.size(); ++i) {
//             if (pollFds[i].revents & POLLIN) {
//                 if (pollFds[i].fd == serverSocket) {
//                     // Handle new client connection
//                     struct sockaddr_in clientAddr;
//                     socklen_t clientAddrLen = sizeof(clientAddr);
//                     int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
//                     if (clientSocket == -1) {
//                         std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
//                     } else {
//                         std::cout << "New client connected" << std::endl;

//                         // Add the new client socket to the pollfd array
//                         struct pollfd clientPollInfo;
//                         clientPollInfo.fd = clientSocket;
//                         clientPollInfo.events = POLLIN;
//                         pollFds.push_back(clientPollInfo);
//                     }
//                 } else {
//                     // Handle incoming data from a connected client
//                     char buffer[4096];
//                     ssize_t bytesRead = recv(pollFds[i].fd, buffer, sizeof(buffer) - 1, 0);
//                     if (bytesRead == -1) {
//                         std::cerr << "Error reading from client socket: " << strerror(errno) << std::endl;
//                     } else if (bytesRead == 0) {
//                         // Client closed the connection
//                         std::cout << "Client disconnected" << std::endl;

//                         // Remove the client socket from the pollfd array and close it
//                         close(pollFds[i].fd);
//                         pollFds.erase(pollFds.begin() + i);
//                     } else {
//                         // Process the received data from the client
//                         buffer[bytesRead] = '\0';
//                         std::cout << "Received from client: " << buffer << std::endl;
//                     }
//                 }
//             }
//         }
//     }

//     // Close the server socket
//     close(serverSocket);
//     return 0;
// }
