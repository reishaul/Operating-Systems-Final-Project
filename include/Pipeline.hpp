
#pragma once
#include "Graph.hpp"
#include "AlgorithmFactory.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>
#include <string>
#include <atomic>

namespace graph {
    extern std::mutex cout_mutex;// mutex for console output
    extern std::atomic<bool> server_running;

struct Job {
    //define job
    std::shared_ptr<Graph> g;
    std::string result;
    std::atomic<bool> completed{false}; // flag to indicate if job is completed

    mutable std::mutex job_mutex; // mutex to protect access to job data
    std::condition_variable cv; // condition variable for job completion

    static std::atomic<size_t> next_id;// for unique job identification
    size_t id;
    Job() : id(++next_id) {}
};
using JobPtr = std::shared_ptr<Job>;//for convenience//new

// BlockingQueue
template<typename T>

//create class BlockingQueue for thread-safe queue
class BlockingQueue {
    std::queue<T> q;//queue to hold items
    mutable std::mutex m;//to protect access to the queue
    std::condition_variable cv;// condition variable for queue operations
    bool is_closed = false;//renamed to avoid conflict
public:
    //function to push items into the queue
    void push(T item) {
        std::unique_lock<std::mutex> lk(m);
        if(is_closed) return;//check if closed
        q.push(std::move(item));
        cv.notify_one();// notify one waiting thread
    }
    //function to pop items from the queue
    T pop() {
        std::unique_lock<std::mutex> lk(m);//to protect access to the queue
        cv.wait(lk, [&]{ return !q.empty() || is_closed; });//wait until queue not empty or closed

        if(q.empty()) return nullptr;//return null if empty and closed
        T item = std::move(q.front());//create item that is moved from the front of the queue
        q.pop();
        return item;
    }

    bool try_pop(T& item) {
        std::unique_lock<std::mutex> lk(m);
        if (q.empty()) return false;
        item = std::move(q.front());
        q.pop();
        return true;
    }

    void close(){
        std::unique_lock<std::mutex> lk(m);
        is_closed = true;
        cv.notify_all();
    }
};

//create class ThreadPool
class ThreadPool {
public:
    //function to push jobs into the input queue
    void pushJob(JobPtr job);

    // Singleton accessor
    static ThreadPool& instance() {
        static ThreadPool pool;
        return pool;
    }

    void shutdown() {
        server_running.store(false);
        q_in.close();
        q_mst.close();
        q_maxflow.close();
        q_ham.close();
        q_clique.close();
    }

    // Destructor to cleanup threads (though they're detached)
    ~ThreadPool() = default;

private:
    ThreadPool(); // private constructor
    void stageWorker(const std::string& algName, BlockingQueue<JobPtr>& in, BlockingQueue<JobPtr>& out);
    void sinkWorker(BlockingQueue<JobPtr>& in);

    BlockingQueue<JobPtr> q_in, q_mst, q_maxflow, q_ham, q_clique;// queues for different stages
};

// Singleton accessor
inline ThreadPool& getThreadPool() {
    return ThreadPool::instance();
}


} 
