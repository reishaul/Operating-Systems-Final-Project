// Pipeline.hpp
#pragma once
#include "Graph.hpp"
#include "AlgorithmFactory.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
//#include <future>
#include <thread>
#include <string>
#include <atomic>

namespace graph {
    extern std::mutex cout_mutex;

struct Job {
    //define job
    std::shared_ptr<Graph> g;
    std::string result;
    std::atomic<bool> completed{false};

    mutable std::mutex job_mutex; // mutex to protect access to job data
    std::condition_variable cv;

    static std::atomic<size_t> next_id;
    size_t id;
    Job() : id(++next_id) {}
};
using JobPtr = std::shared_ptr<Job>;//for convenience//new

// BlockingQueue
template<typename T>

//create class BlockingQueue for thread-safe queue
class BlockingQueue {
    std::queue<T> q;
    mutable std::mutex m;//to protect access to the queue
    std::condition_variable cv;
public:
    //function to push items into the queue
    void push(T item) {
        std::unique_lock<std::mutex> lk(m);
        q.push(std::move(item));
        cv.notify_one();
    }
    //function to pop items from the queue
    T pop() {
        std::unique_lock<std::mutex> lk(m);//to protect access to the queue
        cv.wait(lk, [&]{ return !q.empty(); });//to wait until the queue is not empty
        T item = std::move(q.front());//create item that is moved from the front of the queue
        q.pop();
        return item;
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

private:
    ThreadPool(); // private constructor
    void stageWorker(const std::string& algName, BlockingQueue<JobPtr>& in, BlockingQueue<JobPtr>& out);
    void sinkWorker(BlockingQueue<JobPtr>& in);

    BlockingQueue<JobPtr> q_in, q_mst, q_maxflow, q_ham, q_clique;
};

// Singleton accessor
inline ThreadPool& getThreadPool() {
    return ThreadPool::instance();
}


} 
