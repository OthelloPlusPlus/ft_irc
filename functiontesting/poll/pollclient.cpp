#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create a TCP socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Set server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server IP address
    serverAddr.sin_port = htons(6667); // Replace with the desired port

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(clientSocket);
        return -1;
    }
    std::cout   << "Connected to server\n"
                << "Type your message, or exit to quit"
                << std::endl;

    // Loop to read user input from stdin and send it to the server
    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        // Send the user input to the server
        if (send(clientSocket, input.c_str(), input.length(), 0) == -1) {
            std::cerr << "Error sending message" << std::endl;
        } else {
            std::cout << "Message sent successfully" << std::endl;
        }
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}
