// Pipeline.cpp
#include "Pipeline.hpp"
#include <iostream>

#include <sstream>

namespace graph {

// ThreadPool constructor: start all pipeline threads
ThreadPool::ThreadPool() {
    std::thread(&ThreadPool::stageWorker, this, "MST", std::ref(q_in), std::ref(q_mst)).detach();
    std::thread(&ThreadPool::stageWorker, this, "MAXFLOW", std::ref(q_mst), std::ref(q_maxflow)).detach();
    std::thread(&ThreadPool::stageWorker, this, "HAMILTON", std::ref(q_maxflow), std::ref(q_ham)).detach();
    std::thread(&ThreadPool::stageWorker, this, "MAXCLIQUE", std::ref(q_ham), std::ref(q_clique)).detach();
    std::thread(&ThreadPool::sinkWorker, this, std::ref(q_clique)).detach();
}

// Active Object class
void ThreadPool::pushJob(JobPtr job) {
    q_in.push(std::move(job));

} 

/**
 * @brief Stage worker function that processes jobs for a specific algorithm.
 * @param algName Name of the algorithm to process.
 * @param in Input job queue.
 * @param out Output job queue.
 */
void ThreadPool::stageWorker(const std::string& algName, BlockingQueue<JobPtr>& in, BlockingQueue<JobPtr>& out) {
    auto alg = AlgorithmFactory::create(algName);
    while (true) {
        JobPtr job = in.pop();

        // Print to see that the Job has been taken and is being worked on
        std::cout << "[" << algName << "] starting job on thread " 
                  << std::this_thread::get_id() << std::endl;
        // Run the algorithm on the job's graph
        if (alg) {
            job->result += alg->run(*job->g);
        } else {
            job->result += "ERR UNKNOWN ALGORITHM " + algName + "\n";
        }
        out.push(std::move(job));
        std::cout << "[" << algName << "] running on thread " << std::this_thread::get_id() << std::endl;
    }
}

/**
 * @brief Sink worker function that processes completed jobs.
 * @param in Input job queue.
 */
void ThreadPool::sinkWorker(BlockingQueue<JobPtr>& in) {
    while (true) {
        JobPtr job = in.pop();
        job->done.set_value(std::move(job->result));
    }
}

}
