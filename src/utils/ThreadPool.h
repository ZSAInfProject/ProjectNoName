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
    std::condition_variable jobsCV;

    int scheduledJobs;
    std::mutex scheduledJobsMut;
    std::condition_variable scheduledJobsCV;

    bool shouldTerminate;
    void threadMain();

    static constexpr auto TAG = "ThreadPool";
public:
    void addJob(std::function<void(void)> job);
    void wait();

    explicit ThreadPool(int numberOfThreads);
    ~ThreadPool();
};


#endif //NONAME_THREADPOOL_H
