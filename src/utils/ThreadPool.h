#ifndef NONAME_THREADPOOL_H
#define NONAME_THREADPOOL_H


#include <queue>
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

class ThreadPool {
    std::vector<std::thread> threads;
    std::queue<std::function<void(void)>> jobs;
    std::mutex jobsMut;
    std::atomic<int> scheduledJobs;
    std::condition_variable cond;
    bool shouldTerminate;
    void threadMain();

    static constexpr auto TAG = "ThreadPool";
public:
    void addJob(std::function<void(void)> job);
    bool finished();

    explicit ThreadPool(int numberOfThreads);
    ~ThreadPool();
};


#endif //NONAME_THREADPOOL_H
