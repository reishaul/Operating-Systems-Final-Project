// Pipeline.hpp
#pragma once
#include "Graph.hpp"
#include "AlgorithmFactory.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <future>
#include <thread>
#include <string>

namespace graph {


struct Job {
    std::shared_ptr<Graph> g;
    std::string result;
    std::promise<std::string> done;
};
using JobPtr = std::shared_ptr<Job>;

// BlockingQueue
template<typename T>
class BlockingQueue {
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
public:
    void push(T item) {
        {
            std::lock_guard<std::mutex> lk(m);
            q.push(std::move(item));
        }
        cv.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&]{ return !q.empty(); });
        T item = std::move(q.front());
        q.pop();
        return item;
    }
};

class ThreadPool {
public:
    void pushJob(JobPtr job);

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


} // namespace graph
