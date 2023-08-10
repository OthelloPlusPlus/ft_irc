#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct hostent *serverInfo = gethostbyname("irc.libera.chat");
    if (serverInfo == nullptr) {
        std::cerr << "Error resolving server address" << std::endl;
        close(clientSocket);
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6667);
    memcpy(&serverAddr.sin_addr, serverInfo->h_addr, serverInfo->h_length);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Connected to irc.libera.chat" << std::endl;

    // Send PASS command
    std::string passCommand = "PASS :ft_irc\r\n";
    send(clientSocket, passCommand.c_str(), passCommand.length(), 0);

    // Send USER command
    std::string userCommand = "USER Othe * * :Othello\r\n";
    send(clientSocket, userCommand.c_str(), userCommand.length(), 0);

    // Send NICK command
    std::string nickCommand = "NICK Othe\r\n";
    send(clientSocket, nickCommand.c_str(), nickCommand.length(), 0);

    char buffer[4096];
    while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == -1) {
            std::cerr << "Error receiving data from server" << std::endl;
            break;
        } else if (bytesRead == 0) {
            std::cout << "Server closed the connection" << std::endl;
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Received from server: " << buffer << std::endl;
    }

    close(clientSocket);
    return 0;
}
