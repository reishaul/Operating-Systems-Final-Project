#include "graph.hpp"// Include the Graph class header
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>// For errno
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <atomic>
#include <thread>//for thread using


using namespace graph;
static const int PORT = 5555;// Define the port number for the server
// Define the maximum number of pending connections in the queue
static const int BACKLOG = 16;

// Global flag for graceful shutdown
std::atomic<bool> server_running{true};

// Function to handle terminal input
void handleTerminalInput() {
    std::string input;
    while (server_running.load()) {
        std::getline(std::cin, input);
        
        if (input == "exit" || input == "quit") {
            std::cout << "Shutting down server gracefully..." << std::endl;
            server_running.store(false);
            break;
        } else if (input == "status") {
            std::cout << "Server is running on port " << PORT << std::endl;
        } else if (!input.empty()) {
            std::cout << "Unknown command: " << input << std::endl;
            std::cout << "Available commands: exit, quit, status" << std::endl;
        }
    }
}

/*
    * Reads all text from a file descriptor until EOF.
    * Returns true on success, false on failure.
    * If reading fails, sets err to an error message.
    * The file descriptor must be open for reading.
*/
static bool readAllText(int fd, std::string &out) {
    char buf[4096];// Buffer for reading data
    ssize_t n;
    out.clear();
    while ((n = ::read(fd, buf, sizeof(buf))) > 0) {
        out.append(buf, buf + n);// Append the read data to the output string
    }
    return (n >= 0);
}

/*
    * Writes a string to a file descriptor.
    * Returns true on success, false on failure.
    * The file descriptor must be open for writing.
*/
static bool writeAll(int fd, const std::string &s) {
    size_t sent = 0;// Number of bytes sent
    while (sent < s.size()) {
        ssize_t n = ::write(fd, s.data() + sent, s.size() - sent);// Write data to the file descriptor
        if (n <= 0) return false;
        sent += static_cast<size_t>(n);// Update the number of bytes sent
    }
    return true;
}



/**
 * @brief Handles a client connection
 * Reads the graph from the client, checks for an Eulerian cycle,
 * and sends the result back to the client.
 * @param cfd Client file descriptor.
 */
static void handleClient(int cfd) {
    std::string req;
    if (!readAllText(cfd, req)) {
        writeAll(cfd, "ERR READ_FAILED\n");
        return;
    }

    std::string perr;
    int V = 0, E = 0;
    std::istringstream in(req);
    std::string tag;

    // Read the number of vertices
    if (!(in >> tag) || tag != "V") {
        writeAll(cfd, "ERR PARSE_FAILED: expected 'V <num_vertices>'\n");
        return;
    }
    if (!(in >> V) || V <= 0) {
        writeAll(cfd, "ERR PARSE_FAILED: invalid vertex count\n");
        return;
    }
    if (!(in >> tag) || tag != "E") {
        writeAll(cfd, "ERR PARSE_FAILED: expected 'E <num_edges>'\n");
        return;
    }
    if (!(in >> E) || E < 0) {
        writeAll(cfd, "ERR PARSE_FAILED: invalid edge count\n");
        return;
    }

    Graph G(V);// Create a graph with the specified number of vertices
    for(int i = 0; i < E; ++i) {
        int u, v;
        if (!(in >> u >> v)) {
            writeAll(cfd, "ERR PARSE_FAILED: not enough edge lines\n");
            return;
        }
        try{
            G.addEdge(u, v);// Add the edge to the graph
        } catch (const std::exception &e) {
            writeAll(cfd, "ERR PARSE_FAILED: " + std::string(e.what()) + "\n");
            return;
        }
    }

    // Check if the graph has an Eulerian cycle
    if (!G.has_eulerian_cycle()) {
        writeAll(cfd, "ERR NO_EULERIAN_CYCLE\n");
        return;
    }

    auto circuit = G.get_eulerian_cycle();

    if (circuit.empty()) {// If no Eulerian cycle is found
        writeAll(cfd, "ERR NO_EULERIAN_CYCLE\n");
        return;
    }

    std::ostringstream out;
    out << "OK";
    // Write the result to the output stream
    for (int v : circuit) out << ' ' << v;
    out << "\n";
    writeAll(cfd, out.str());
}

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
    std::cerr << "Type exit to stop the server gracefully\n";

    // Create thread to handle terminal input
    std::thread terminal_thread(handleTerminalInput);
    terminal_thread.detach();

    // Main loop to accept and handle client connections
    // Accept connections in a loop and handle each client in the handleClient function
    while (server_running.load()) {
        sockaddr_in cli{}; 
        socklen_t clilen = sizeof(cli);// Client address structure

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        int cfd = accept(sfd, (sockaddr*)&cli, &clilen);// Accept a client connection
        if (cfd < 0) {
            if (errno == EINTR|| errno == EAGAIN || errno == EWOULDBLOCK){
                continue;// Timeout or interrupt - check server_running again
            } 
            if(server_running.load()){
                perror("accept");
            }
            break;
        }
        if(!server_running.load()){
            close(cfd);
            break;
        }
        handleClient(cfd);// Handle the client connection
        close(cfd);// Close the client connection
    }

    close(sfd);// Close the server socket
    std::cerr << "Server shut down gracefully.\n";
    return 0;
}
