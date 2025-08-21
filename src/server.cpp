#include "Graph.hpp"// Include the Graph class header
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
#include <thread>//for thread using

//for generate random graph
#include <random>
#include <unordered_set>

//for part 9 pipeline
#include "Pipeline.hpp"




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
    /**I am using this line to check if the server support multi-threading
     *if we run two or more requests in two different terminals we can see that both
     *of the requests are being processed simultaneously and finishing after five seconds
     *(In single thread mode, the requests would finish one after 5 second and the second after
     *more 5 seconds and we get 10 seconds for the whole process)
    std::this_thread::sleep_for(std::chrono::seconds(5));
    */


    std::string req;
    if (!readAllText(cfd, req)) {
        writeAll(cfd, "ERR READ_FAILED\n");
        return;
    }

    int V , E;
    std::istringstream in(req);
    std::string tag;

    //for part 8 b
    if (!(in >> tag)) {
        writeAll(cfd, "ERR PARSE_FAILED: missing request type\n");
        return;
    }

    bool randomGraph = false;
    if (tag == "GRAPH") {
        //if the client requested a specific graph and continue to read its description
    }

    else if (tag == "RANDOM") {//if the client requested a random graph
        randomGraph = true;
    }

    else {
        writeAll(cfd, "ERR PARSE_FAILED: expected 'GRAPH' or 'RANDOM'\n");
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
    //bool withWeight = (algo == "MST" || algo == "MAXFLOW");

    if(randomGraph){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, V-1);

        std::unordered_set<long long> used; // to prevent duplicates
        std::uniform_int_distribution<> wdist(1, 10);//range of random weights

        for (int i = 0; i < E; ++i) {
            int u, v;
            long long key;
            do {
                u = dist(gen);
                v = dist(gen);
                key = (static_cast<long long>(u) << 32) | v;
            } while (u == v || used.count(key)); // without self-loops and duplicates

            used.insert(key);
            G.addEdge(u, v, wdist(gen)); // random weight
        }
    }
    else{
        for (int i = 0; i < E; ++i) {
            int u, v, w = 1;
            if (!(in >> u >> v)) {
                writeAll(cfd, "ERR PARSE_FAILED: invalid edge line format\n");
                return;
            }
            if (in.peek() != '\n' && in >> w) {
                //if there is a weight, read it
            }
            G.addEdge(u, v, w);
        }
    }

// Pipeline 

    auto job = std::make_shared<graph::Job>();
    job->g = std::make_shared<Graph>(G);
    std::future<std::string> fut = job->done.get_future();

    graph::getThreadPool().pushJob(job);

    std::string response = fut.get(); // wait to the algorithms to finish
    writeAll(cfd, response);

    // std::vector<std::string> algos = {"MST", "MAXFLOW", "HAMILTON","MAXCLIQUE"};
    // std::ostringstream out;

    // for (auto &a : algos) {
    //     if (auto alg = graph::AlgorithmFactory::create(a)) {
    //         out << alg->run(G);
    //     } else {
    //         out << "ERR UNKNOWN ALGORITHM " << a << "\n";
    //     }
    // }

    // writeAll(cfd, out.str());

}


