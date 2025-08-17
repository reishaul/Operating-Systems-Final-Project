//Rei Shaul E-mail: reishaul1@gmail.com
#include <iostream>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <unistd.h>  // For getopt  
#include "Graph.hpp"

int main(int argc, char* argv[]) {
    int num_vertices = 0;
    int num_edges = 0;
    int seed =time(nullptr); // Seed the random number generator with the current time

    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:")) != -1) {
        switch (opt) {
            case 'v':
                num_vertices = std::stoi(optarg);
                break;
            case 'e':
                num_edges = std::stoi(optarg);
                break;
            case 's':
                seed = std::stoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -v <num_vertices> -e <num_edges> -s <seed>\n";
                return 1;
        }
    }

    if(num_vertices <= 0 || num_edges < 0) {
        std::cerr << "Invalid number of vertices or edges.\n";
        return 1;
    }
    graph::Graph gr(num_vertices);
    std::mt19937 rng(seed); // Initialize random number generator with the seed
    std::uniform_int_distribution<int> dist(0, num_vertices - 1);

    for (int i=0; i<num_edges; ++i) {
        int u = dist(rng);
        int v = dist(rng);

        if(u==v){
            --i; // prevent self-loops
            continue; // Skip this iteration if self-loop is generated
        }
        
        // Check if edge already exists
        bool edgeExists = false;
        auto edges = gr.get_edges();
        for(const auto& edge : edges) {
            if((std::get<0>(edge) == u && std::get<1>(edge) == v) ||
               (std::get<0>(edge) == v && std::get<1>(edge) == u)) {
                edgeExists = true;
                break;
            }
        }
        
        if(edgeExists){
            --i; // prevent duplicate edges
            continue; // Skip this iteration if the edge already exists
        }
        gr.addEdge(u, v);
        
    }
    std::cout << "Graph with " << num_vertices << " vertices and " << num_edges << " edges created\n";

    gr.print_graph();
    auto ecircuit = gr.get_eulerian_cycle();
    if (ecircuit.empty()) {
        std::cout << "No Eulerian circuit found\n";
    } 
    else {
        std::cout << "Eulerian circuit found: ";
        for (int v : ecircuit) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }

    std::cout << "some changes in the graph:\n";
    // Test removeEdge functionality
    if (num_edges > 0) {
        auto edges = gr.get_edges();
        if (!edges.empty()) {
            auto first_edge = edges[0];
            int src = std::get<0>(first_edge);
            int dest = std::get<1>(first_edge);
            std::cout << "Removing edge (" << src << ", " << dest << ")\n";
            gr.removeEdge(src, dest);
            std::cout << "Edge removed successfully.\n";
            
            // Try to remove non-existent edge to test exception
            try {
                gr.removeEdge(src, dest); // This should throw an exception
            } catch (const std::runtime_error& e) {
                std::cout << "Expected error caught: " << e.what() << "\n";
            }
        }
    }

    // Test invalid constructor
    try {
        graph::Graph invalid_graph(-1);
    } catch (const std::invalid_argument& e) {
        std::cout << "Expected error caught: " << e.what() << "\n";
    }

    // Test invalid vertex access
    try {
        gr.addEdge(-1, 0); // This should throw out_of_range
    } catch (const std::out_of_range& e) {
        std::cout << "Expected error caught: " << e.what() << "\n";
    }

    // Create a disconnected graph to test isConnected
    graph::Graph disconnected_graph(5);
    disconnected_graph.addEdge(0, 1);
    disconnected_graph.addEdge(2, 3);
    // vertex 4 is isolated, vertices 0-1 and 2-3 are in separate components
    try {
        auto circuit = disconnected_graph.get_eulerian_cycle();
    } catch (const std::runtime_error& e) {
        std::cout << "Expected error for disconnected graph: " << e.what() << "\n";
    }

    // Test get_eulerian_cycle with graph that doesn't have Eulerian cycle
    try {
        auto ecircuit = gr.get_eulerian_cycle();
        if (ecircuit.empty()) {
            std::cout << "No Eulerian circuit found\n";
        } else {
            std::cout << "Eulerian circuit found: ";
            for (int v : ecircuit) {
                std::cout << v << " ";
            }
            std::cout << "\n";
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Expected error caught: " << e.what() << "\n";
    }

    return 0;
}