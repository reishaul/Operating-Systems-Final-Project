//Rei Shaul E-mail: reishaul1@gmail.com
#include "server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <cerrno>

#include <thread>//for thread using

static const int PORT = 5555;
static const int BACKLOG = 16;

int main() {
    int sfd = ::socket(AF_INET, SOCK_STREAM, 0);// Create a socket for the server(ipv4, TCP)
    if (sfd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));// Set socket options to reuse the address

    // Set up the server address structure
    sockaddr_in addr{};// Server address 
    addr.sin_family = AF_INET;// IPv4 address family
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(sfd, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }// Bind the socket to the address and port
    if (listen(sfd, BACKLOG) < 0) { perror("listen"); return 1; }// Listen for incoming connections

    std::cerr << "Server listening on port " << PORT << " ...\n";

    // Main loop to accept and handle client connections
    // Accept connections in a loop and handle each client in the handleClient function
    while (true) {
        sockaddr_in cli{};
        socklen_t clilen = sizeof(cli);// Client address structure
        int cfd = accept(sfd, (sockaddr*)&cli, &clilen);// Accept a client connection
        if (cfd < 0) {
            if (errno == EINTR) continue;// Interrupted system call
            perror("accept");
            break;
        }
        //for thread
        //this 4 lines are for creating a new thread to handle the client connection
        std::thread t([cfd]() {
            handleClient(cfd);
            close(cfd);
        });
        t.detach(); // Run in the background without join

    }

    close(sfd);// Close the server socket
    return 0;
}