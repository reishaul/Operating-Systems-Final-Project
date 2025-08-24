//Rei Shaul E-mail: reishaul1@gmail.com
#include "server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <cerrno>

#include <thread>//for thread using
#include <vector>
#include <atomic>
#include <algorithm> // for std::remove_if
#include <signal.h> // for signal handling
#include <csignal> // for signal handling
#include <chrono>
#include <string>

#include <Pipeline.hpp>

static const int PORT = 5555;
static const int BACKLOG = 16;

std::atomic<bool> server_running{true};//variable to control server status

// Signal handler for graceful shutdown
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\nReceived shutdown signal. Shutting down server gracefully..." << std::endl;
        server_running.store(false);
    }
}

// Function to handle terminal input in a separate thread
void handleTerminalInput() {
    std::string input;
    while (server_running.load()) {
        if (std::getline(std::cin, input)) {
            if (input == "exit" || input == "quit") {
                std::cout << "Shutting down server gracefully..." << std::endl;
                server_running = false;

                graph::getThreadPool().shutdown();
                break;
            } 
            else if (input == "help") {
                std::cout << "Available commands: exit, quit, status, help" << std::endl;
            } 
            else if (!input.empty()) {
                std::cout << "Unknown command: '" << input << "'. Type 'help' for available commands." << std::endl;
            }
        }
        
        // Small delay to prevent busy waiting
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // Set up signal handlers for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
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
    std::cout << "Type 'exit' to shutdown gracefully, or use Ctrl+C" << std::endl;

    // Start terminal input handler thread
    std::thread input_thread(handleTerminalInput);

    std::vector<std::thread> threads;

    // Main loop to accept and handle client connections
    while (server_running.load()) {
        sockaddr_in cli{};
        socklen_t clilen = sizeof(cli);// Client address structure
        
        // Set timeout for accept to periodically check server_running
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        int cfd = accept(sfd, (sockaddr*)&cli, &clilen);// Accept a client connection
        if (cfd < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;// Timeout או interrupt - בדוק שוב את server_running
            }
            if (server_running.load()) {
                perror("accept");
            }
            break;
        }
        
        if (!server_running.load()) {
            close(cfd);
            break;
        }
        
        // Create thread to handle client
        threads.emplace_back([cfd]() {
            handleClient(cfd);
            close(cfd);
        });
    }

    std::cout << "Shutting down server..." << std::endl;
    
    // Join all threads before exiting
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    
    // Join input thread
    if (input_thread.joinable()) {
        input_thread.join();
    }

    close(sfd);// Close the server socket
    
    std::cout << "Server shutdown complete." << std::endl;
    
    return 0;
}