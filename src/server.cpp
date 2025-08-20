#include "Graph.hpp"// Include the Graph class header
#include "Digraph.hpp"// Include the Digraph class header
#include "AlgorithmFactory.hpp"// Include the AlgorithmFactory header
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
#include "server.hpp"


using namespace graph;
static const int PORT = 5555;// Define the port number for the server
// Define the maximum number of pending connections in the queue
static const int BACKLOG = 16;


/*
    * Reads all text from a file descriptor until EOF.
    * Returns true on success, false on failure.
    * If reading fails, sets err to an error message.
    * The file descriptor must be open for reading.
*/
bool readAllText(int fd, std::string &out) {
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
bool writeAll(int fd, const std::string &s) {
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
void handleClient(int cfd) {
    std::string req;
    if (!readAllText(cfd, req)) {
        writeAll(cfd, "ERR READ_FAILED\n");
        return;
    }

    std::string perr;
    int V , E;
    std::istringstream in(req);
    std::string tag, algo;

    // Read the number of vertices
    if (!(in >> tag) || tag != "ALG") {
        writeAll(cfd, "ERR PARSE_FAILED: expected 'ALG <algorithm>'\n");
        return;
    }
    if (!(in >> algo) || algo.empty()) {
        writeAll(cfd, "ERR PARSE_FAILED: invalid algorithm name\n");
        return;
    }

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
   
    bool withWeight = (algo == "MST" || algo == "MAXFLOW");

    for(int i = 0; i < E; ++i) {
        int u,v,w=1;//default weight is 1
        if(withWeight){
            if(!(in>>u>>v>>w)){
                writeAll(cfd, "ERR PARSE_FAILED: invalid edge line format\n");
                return;
            }
            G.addEdge(u, v, w);// Add the edge with weight to the graph
        }
        else{
            if(!(in>>u>>v)){
                writeAll(cfd, "ERR PARSE_FAILED: invalid edge line format\n");
                return;
            }
            G.addEdge(u, v);// Add the edge without weight to the graph
        }
    }


    if (auto alg = graph::AlgorithmFactory::create(algo)) {
        writeAll(cfd, alg->run(G));
    }
    else {
        writeAll(cfd, "ERR UNKNOWN ALGORITHM\n");
    }  

}


