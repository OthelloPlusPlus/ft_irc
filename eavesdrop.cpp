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
#include <netdb.h>
// struct hostent	*gethostbyname(const char *name);
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <poll.h>
#include <netdb.h>

#include "colors.hpp"

typedef struct {
	int socketFd;
	struct sockaddr_in socketAddress;
	socklen_t socketAddressLen;
	struct pollfd pollInfo;
} t_connect;

bool	setServer(t_connect &server);
bool	preparePidgin(t_connect &pidgin);
bool	prepareLibera(t_connect &libera);
bool	setSocket(int &socketFd);
bool	pollThis(t_connect &connection);
void	acceptPidgin(t_connect &server, t_connect &pidgin);
void	connectLibera(t_connect &libera);
void	recvAndSend(t_connect &source, t_connect &destination, std::string color);

int	main(void)
{
	t_connect	server;
	t_connect	pidgin;
	t_connect	libera;

	if (!setServer(server))
		return (1);
	if (!preparePidgin(pidgin))
		return (1);
	if (!prepareLibera(libera))
		return (1);
	while (true)
	{
		if (!pollThis(server))
			break ;
		if (server.pollInfo.revents & POLLIN)
		{
			acceptPidgin(server, pidgin);
			connectLibera(libera);
		}
		if (!pollThis(pidgin) || !pollThis(libera))
			break ;
		if (pidgin.socketFd > 2 && pidgin.pollInfo.revents & POLLIN)
			recvAndSend(pidgin, libera, C_CYAN);
		if (libera.socketFd > 2 && libera.pollInfo.revents & POLLIN)
			recvAndSend(libera, pidgin, C_ORANGE);
	}
}

bool	setServer(t_connect &server)
{
	server.socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (server.socketFd < 0)
	{
		std::cerr << "Error socket(): " << strerror(errno) << std::endl;
		return false;
	}

	server.socketAddress.sin_family = AF_INET;
	server.socketAddress.sin_port = htons(6667);
	server.socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");; // Listen on any available interface

	if (bind(server.socketFd, (struct sockaddr *)&server.socketAddress, sizeof(server.socketAddress)) == -1)
	{
		std::cerr << "Error bind(): " << strerror(errno) << std::endl;
		close(server.socketFd);
		return false;
	}

	if (listen(server.socketFd, SOMAXCONN) == -1)
	{
		std::cerr << "Error listen(): " << strerror(errno) << std::endl;
		close(server.socketFd);
		return false;
	}

	server.pollInfo.fd = server.socketFd;
	server.pollInfo.events = POLLIN;

	return true;
}

bool	preparePidgin(t_connect &pidgin)
{
	if (!setSocket(pidgin.socketFd))
		return false;

	pidgin.socketAddress.sin_family = AF_INET;
	pidgin.socketAddress.sin_port = htons(6667);
	pidgin.socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	pidgin.pollInfo.fd = pidgin.socketFd;
	pidgin.pollInfo.events = POLLIN;

	return true;
}
bool	prepareLibera(t_connect &libera)
{
	if (!setSocket(libera.socketFd))
		return false;

	struct hostent *liberaInfo;
	libera.socketAddress.sin_family = AF_INET;
	libera.socketAddress.sin_port = htons(6667);
	liberaInfo = gethostbyname("irc.libera.chat");
	memcpy(&libera.socketAddress.sin_addr.s_addr, liberaInfo->h_addr, liberaInfo->h_length);

	libera.pollInfo.fd = libera.socketFd;
	libera.pollInfo.events = POLLIN;

	return true;
}
bool	setSocket(int &socketFd)
{
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
	{
		std::cerr	<< "Error socket(): "	<< strerror(errno)	<< std::endl;
		return (false);
	}
	if (fcntl(socketFd,  F_SETFL, fcntl(socketFd, F_GETFL, 0) | O_NONBLOCK) == -1)
	{
		std::cerr	<< "Error fcntl(): "	<< strerror(errno)	<< std::endl;
		close(socketFd);
		return (false);
	}
	return (true);
}

bool	pollThis(t_connect &connection)
{
	if (poll(&connection.pollInfo, 1, 0) < 0)
	{
		std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
		return (false);
	}
	return (true);
}

void	acceptPidgin(t_connect &server, t_connect &pidgin)
{
	pidgin.socketFd = accept(server.socketFd, (struct sockaddr *)&pidgin.socketAddress, &pidgin.socketAddressLen);
	if (pidgin.socketFd == -1)
		std::cerr << "Error accept(): " << strerror(errno) << std::endl;
	else
	{
		std::cout << "Pidgin connection accepted!" << std::endl;
		pidgin.pollInfo.fd = pidgin.socketFd; // Update the pollInfo for pidgin
		pidgin.pollInfo.events = POLLIN | POLLOUT; // Add POLLOUT event for writing
	}
}

void	connectLibera(t_connect &libera)
{
	int liberaSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (liberaSocket < 0)
	{
		std::cerr << "Error socket() (libera): " << strerror(errno) << std::endl;
		return;
	}

	if (connect(liberaSocket, (struct sockaddr *)&libera.socketAddress, sizeof(libera.socketAddress)) < 0)
	{
		if (errno != EINPROGRESS)
		{
			std::cerr << "Error connect() libera: " << strerror(errno) << std::endl;
			close(liberaSocket);
			return;
		}
	}

	struct pollfd pollFds[1];
	pollFds[0].fd = liberaSocket;
	pollFds[0].events = POLLOUT;

	int pollResult = poll(pollFds, 1, 5000); // Wait up to 5 seconds
	if (pollResult <= 0 || (pollFds[0].revents & POLLOUT) == 0)
	{
		std::cerr << "Error connecting to libera: Connection timeout or error." << std::endl;
		close(liberaSocket);
		return;
	}

	libera.socketFd = liberaSocket;
	libera.pollInfo.fd = libera.socketFd;
	libera.pollInfo.events = POLLIN | POLLOUT; // Add POLLOUT event
	std::cout	<< "Connected to libera!"	<< std::endl;
}

void	recvAndSend(t_connect &source, t_connect &destination, std::string color)
{
	char buffer[4096];
	ssize_t readLen = recv(source.socketFd, buffer, sizeof(buffer) - 1, 0);
		
	if (readLen < 0)
		std::cerr << "Error recv(): " << strerror(errno) << std::endl;
	else if (readLen == 0)
	{
		close(source.socketFd);
		source.socketFd = -1;
		std::cout << "Connection closed: " << source.socketFd << std::endl;
	}
	else
	{
		buffer[readLen] = '\0'; // Null-terminate the received data
		std::cout << "Received:\n" << color	<< buffer << C_RESET	<< std::endl;

		ssize_t sentLen = send(destination.socketFd, buffer, readLen, 0);
		if (sentLen < 0)
			std::cerr << "Error sending data: " << strerror(errno) << std::endl;
		else
			std::cout << "Sent: " << sentLen << " bytes" << std::endl;
	}
}

// #include <poll.h>
// //	int	poll(struct pollfd fds[], nfds_t nfds, int timeout);
// #include <iostream>
// //	std::cout
// #include <sys/socket.h>
// //	ssize_t	recv(int socket, void *buffer, size_t length, int flags);
// //	int	bind(int socket, const struct sockaddr *address, socklen_t address_len);
// //	int	listen(int socket, int backlog);
// #include <unistd.h>
// //	int	close(int fildes);
// #include <sys/socket.h>
// //	int	socket(int domain, int type, int protocol);
// #include <netinet/in.h>
// //	struct sockaddr_in
// #include <arpa/inet.h>
// //	in_addr_t	inet_addr(const char *cp);
// //	uint16_t	htons(uint16_t hostshort);
// //	struct sockaddr_in
// #include <fcntl.h>
// //	int	fcntl(int fildes, int cmd, ...);
// #include <netdb.h>
// // struct hostent	*gethostbyname(const char *name);

// typedef struct	s_connect
// {
// 	int					socketFd;
// 	struct sockaddr_in	socketAddress;
// 	socklen_t			socketAddressLen;
// 	struct pollfd		pollInfo;
// }	t_connect;

// bool	st_setup(t_connect &server, t_connect &pidgin, t_connect &libera);
// void	acceptPidgin(t_connect &server, t_connect &pidgin);
// void	connectLibera(t_connect &server, t_connect &libera);
// void	fromPidginToLibera(t_connect &pidgin, t_connect &libera);
// void	fromLiberaToPidgin(t_connect &libera, t_connect &pidgin);
// bool	setSocket(int &socketFd);
// void	checkPidginPollin(t_connect &pidgin, t_connect &libera);

// int	main(void)
// {
// 	t_connect	server;
// 	t_connect	pidgin;
// 	t_connect	libera;

// 	if (!st_setup(server, pidgin, libera))
// 		return (true);
// 	while (true)
// 	{
// 		if (poll(&server.pollInfo, 1, 0) < 0)
// 		{
// 			std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
// 			break ;
// 		}
// 		if (server.pollInfo.revents & POLLIN)
// 		{
// 			acceptPidgin(server, pidgin);
// 			connectLibera(server, libera);
// 		}
// 		checkPidginPollin(pidgin, libera);
// 		// if (poll(&libera.pollInfo, 1, 0) < 0)
// 		// {
// 		// 	std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
// 		// 	break ;
// 		// }
// 		// if (libera.pollInfo.revents & POLLIN)
// 		// 	fromLiberaToPidgin(libera, pidgin);
// 	}
// }

// bool	st_setup(t_connect &server, t_connect &pidgin, t_connect &libera)
// {
// 	if (!setSocket(server.socketFd))
// 		return (false);
// 	server.socketAddress.sin_family = AF_INET;
// 	server.socketAddress.sin_port = htons(6667);
// 	server.socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
// 	if (bind(server.socketFd, (struct sockaddr *)&server.socketAddress, sizeof(server.socketAddress)) == -1)
// 	{
// 		std::cerr	<< "Error bind(): "	<< strerror(errno)	<< std::endl;
// 		close(server.socketFd);
// 		return (false);
// 	}
// 	if (listen(server.socketFd, SOMAXCONN) == -1)
// 	{
// 		std::cerr	<< "Error listen(): "	<< strerror(errno)	<< std::endl;
// 		close(server.socketFd);
// 		return (false);
// 	}
// 	server.pollInfo.fd = server.socketFd;
// 	server.pollInfo.events = POLLIN;

// 	pidgin.socketAddressLen = sizeof(pidgin.socketAddress);
// 	pidgin.pollInfo.events = POLLIN;

// 	if (!setSocket(libera.socketFd))
// 		return (false);
// 	struct hostent	*liberaInfo;
// 	libera.socketAddress.sin_family = AF_INET;
// 	libera.socketAddress.sin_port = htons(6667);
// 	liberaInfo = gethostbyname("irc.libera.chat");
// 	memcpy(&libera.socketAddress.sin_addr.s_addr, liberaInfo->h_addr, liberaInfo->h_length);
// 	libera.pollInfo.fd = libera.socketFd;
// 	return (true);
// }

// bool	setSocket(int &socketFd)
// {
// 	socketFd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (socketFd < 0)
// 	{
// 		std::cerr	<< "Error socket(): "	<< strerror(errno)	<< std::endl;
// 		return (false);
// 	}
// 	if (fcntl(socketFd,  F_SETFL, fcntl(socketFd, F_GETFL, 0) | O_NONBLOCK) == -1)
// 	{
// 		std::cerr	<< "Error fcntl(): "	<< strerror(errno)	<< std::endl;
// 		close(socketFd);
// 		return (false);
// 	}
// 	return (true);
// }

// void	acceptPidgin(t_connect &server, t_connect &pidgin)
// {
// 	pidgin.socketFd = accept(server.socketFd, (struct sockaddr *)&pidgin.socketAddress, &pidgin.socketAddressLen);
// 	if (pidgin.socketFd == -1)
// 		std::cerr	<< "Error accept(): "	<< strerror(errno)	<< std::endl;
// 	else
// 		std::cout	<< "Pidgin connection accepted!"	<< std::endl;
// 	pidgin.pollInfo.fd = pidgin.socketFd;
// }

// void	connectLibera(t_connect &server, t_connect &libera)
// {
// 	connect(libera.socketFd, (struct sockaddr *)&libera.socketAddress, sizeof(libera.socketAddress));
// }

// void	checkPidginPollin(t_connect &pidgin, t_connect &libera)
// {
// 	if (poll(&pidgin.pollInfo, 1, 0) < 0)
// 	{
// 		std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
// 		return ;
// 	}
// 	if (pidgin.pollInfo.revents & POLLIN)
// 	{
// 		char	buffer[4096];
// 		size_t	readLen;

// 		readLen = recv(pidgin.pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
// 		if (readLen < 0)
// 			std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
// 		else if (readLen == 0)
// 		{
// 			close(pidgin.pollInfo.fd);
// 			pidgin.pollInfo.fd = 0;
// 			std::cout	<< "Pidgin disconnected from server."	<< std::endl;
// 		}
// 		else
// 		{
// 			std::cout	<< "message received:\n"
// 						<< buffer	<< std::endl;
// 			if (poll(&libera.pollInfo, 1, 5000) < 0)
// 			{
// 				std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
// 				return ;
// 			}
// 			if (libera.pollInfo.revents & POLLOUT)
// 				std::cout	<< "socket ready"	<< std::endl;
// 			else
// 				std::cout	<< "socket not connected"	<< std::endl;
// 			if (send(libera.pollInfo.fd, buffer, readLen, 0) > 0)
// 				std::cout	<< "Message forwarded."	<< std::endl;
// 			else
// 				std::cerr	<< "Failed to forward message: "	<< strerror(errno)	<< std::endl;
// 		}
// 	}
// }

// void	fromPidginToLibera(t_connect &pidgin, t_connect &libera)
// {
// 	std::cout	<< __func__	<< __LINE__	<< std::endl;
// 	char	buffer[4096];
// 	size_t	readLen;

// 	readLen = recv(pidgin.pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
// 	if (readLen < 0)
// 		std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
// 	else if (readLen == 0)
// 	{
// 		close (pidgin.pollInfo.fd);
// 		pidgin.pollInfo.fd = 0;
// 		std::cout	<< "Pidgin disconnected from server."	<< std::endl;
// 	}
// 	else
// 	{
// 		std::cout	<< "Message from pigin received:\n"
// 					<< buffer	<< std::endl;
// 		send(libera.socketFd, buffer, strlen(buffer), 0);
// 	}

// }

// void	fromLiberaToPidgin(t_connect &libera, t_connect &pidgin)
// {
// 	std::cout	<< __func__	<< __LINE__	<< std::endl;
// 	char	buffer[4096];
// 	size_t	readLen;

// 	readLen = recv(libera.pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
// 	if (readLen < 0)
// 		std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
// 	else if (readLen == 0)
// 	{
// 		close (libera.pollInfo.fd);
// 		libera.pollInfo.fd = 0;
// 		std::cout	<< "Libera disconnected from server."	<< std::endl;
// 	}
// 	else
// 	{
// 		std::cout	<< "Message from pigin received:\n"
// 					<< buffer	<< std::endl;
// 		send(pidgin.socketFd, buffer, strlen(buffer), 0);
// 	}	
// }