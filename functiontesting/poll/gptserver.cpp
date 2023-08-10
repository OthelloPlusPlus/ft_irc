#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>

int main() {
    // Create server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating server socket" << std::endl;
        return 1;
    }

    // Set server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(6667);  // IRC default port

    // Bind server socket to the address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding server socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Error listening on server socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Create a vector to store pollfd structures
    std::vector<struct pollfd> pollFds;

    // Add the server socket to the pollfd array
    struct pollfd serverPollInfo;
    serverPollInfo.fd = serverSocket;
    serverPollInfo.events = POLLIN;
    pollFds.push_back(serverPollInfo);

    std::cout << "Server is running and listening for incoming connections..." << std::endl;

    bool confirmed = false;  // To track if connection is confirmed

    while (true) {
        int ready = poll(pollFds.data(), pollFds.size(), -1);
        if (ready == -1) {
            std::cerr << "Error in poll: " << strerror(errno) << std::endl;
            break;
        }

        // Handle server socket event (new connection)
        if (pollFds[0].revents & POLLIN) {
            // Accept new client connection
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
            if (clientSocket == -1) {
                std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
            } else {
                std::cout << "New client connected" << std::endl;

                // Add the new client socket to the pollfd array
                struct pollfd clientPollInfo;
                clientPollInfo.fd = clientSocket;
                clientPollInfo.events = POLLIN;
                pollFds.push_back(clientPollInfo);
            }
        }

        // Handle client socket events (data received)
        for (size_t i = 1; i < pollFds.size(); ++i) {
            if (pollFds[i].revents & POLLIN) {
                // Handle incoming data from clients
                char buffer[4096];
                ssize_t bytesRead = recv(pollFds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead == -1) {
                    std::cerr << "Error reading from client socket: " << strerror(errno) << std::endl;
                } else if (bytesRead == 0) {
                    // Client closed the connection
                    std::cout << "Client disconnected" << std::endl;

                    // Remove the client socket from the pollfd array and close it
                    close(pollFds[i].fd);
                    pollFds.erase(pollFds.begin() + i);
                } else {
                    // Process the received data from the client
                    buffer[bytesRead] = '\0';
                    std::cout << "Received from client: " << buffer << std::endl;

                    // Simulate confirming the connection and initiating a chat
                    if (!confirmed) {
                        std::string rpl_welcome = ":server 001 othello :Welcome to the IRC server, othello!\r\n";
                        send(pollFds[i].fd, rpl_welcome.c_str(), rpl_welcome.length(), 0);

                        std::string privmsg = "PRIVMSG #mychannel :Hello, this is a test message!\r\n";
                        send(pollFds[i].fd, privmsg.c_str(), privmsg.length(), 0);

                        confirmed = true;
                    }

                    // Handle client responses or other interactions
                    // ... (process received data, handle chat responses, etc.)
                }
            }
        }
    }

    // Close the server socket
    close(serverSocket);
    return 0;
}
