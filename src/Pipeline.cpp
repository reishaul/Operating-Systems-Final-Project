#include "Pipeline.hpp"
#include <iostream>
#include <sstream>


namespace graph {
    extern std::atomic<bool> server_running{true};
    std::mutex cout_mutex;// mutex for console output
    std::atomic<size_t> Job::next_id{0};// for unique job identification

#define SAFE_COUT(x) do{std::lock_guard<std::mutex> lk(cout_mutex); std::cerr << x << std::endl;} while(0)// safe console output

// ThreadPool constructor: start all pipeline threads
ThreadPool::ThreadPool() {
    std::thread(&ThreadPool::stageWorker, this, "MST", std::ref(q_in), std::ref(q_mst)).detach();
    std::thread(&ThreadPool::stageWorker, this, "MAXFLOW", std::ref(q_mst), std::ref(q_maxflow)).detach();
    std::thread(&ThreadPool::stageWorker, this, "HAMILTON", std::ref(q_maxflow), std::ref(q_ham)).detach();
    std::thread(&ThreadPool::stageWorker, this, "MAXCLIQUE", std::ref(q_ham), std::ref(q_clique)).detach();
    std::thread(&ThreadPool::sinkWorker, this, std::ref(q_clique)).detach();
}

// Active Object class
/*
 * @brief Pushes a 'job' into the input queue.
 * @param job The job to be pushed
 */
void ThreadPool::pushJob(JobPtr job) {
    q_in.push(std::move(job));
} 

/**
 * @brief Stage worker function that processes jobs for a specific algorithm
 * @param algName Name of the algorithm to process.
 * @param in Input job queue
 * @param out Output job queue.
 */
void ThreadPool::stageWorker(const std::string& algName, BlockingQueue<JobPtr>& in, BlockingQueue<JobPtr>& out) {
    auto alg = AlgorithmFactory::create(algName);//create algorithm instance
    while (server_running.load()) {
        JobPtr job = in.pop();//get job from input queue
        if(!job) break;//new

        {// Print to see that the Job has been taken and is being worked on
            std::lock_guard<std::mutex> lk(cout_mutex);
            std::cerr << "[" << algName << "] starting job " << job->id
                      << " on thread " << std::this_thread::get_id() << std::endl;
        }

        // Run the algorithm on the job's graph
        std::string result_part;
        if (alg) {
            result_part = alg->run(*job->g);
        } 
        else {
            result_part = "ERR UNKNOWN ALGORITHM " + algName + "\n";
        }
        
        // Protect access to shared result
        {
            std::lock_guard<std::mutex> lk(job->job_mutex);//lock_guard is used to protect access to job data
            job->result += result_part;
        }

        // Lock before writing to result
        {
            std::lock_guard<std::mutex> lk(cout_mutex);
            std::cerr << "[" << algName << "] job " << job->id
                      << " moving to next stage " << std::endl;
        }

        out.push(std::move(job));//push job to output queue
    }
}

/**
 * @brief Sink worker function that processes completed jobs
 * @param in Input job queue.
 */
void ThreadPool::sinkWorker(BlockingQueue<JobPtr>& in) {
        while (server_running.load()) {

            JobPtr job = in.pop();
            if(!job) break;//new

            SAFE_COUT("sinkWorker: processing job " << job->id);//safe console output

            {
                std::lock_guard<std::mutex> lk(job->job_mutex);//lock_guard is used to protect access to job data
                job->completed.store(true);// mark job as completed
                job->cv.notify_one();// notify waiting threads
            }
            SAFE_COUT("sinkWorker: notified job " << job->id);
        }
    }
}
