#ifndef NONAME_THREADPOOL_H
#define NONAME_THREADPOOL_H


#include <queue>
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

//! Class representing thread pool
/*!
 * This class can be used when you want to
 * easily execute jobs in parallel or reuse
 * threads.
 */
class ThreadPool {
    
public:
    static constexpr auto TAG = "ThreadPool";

    //! Add job to job queue
    /*!
     * addJob() adds a job to the job queue where
     * it waits to be executed by any free thread.
     * @param job
     */
    void addJob(std::function<void(void)> job);
    //! Waits until all jobs are completed.
    void wait();

    explicit ThreadPool(int numberOfThreads);
    ~ThreadPool();

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void(void)>> jobs;
    std::mutex jobsMut;
    std::condition_variable jobsCV;

    int scheduledJobs;
    std::mutex scheduledJobsMut;
    std::condition_variable scheduledJobsCV;

    bool shouldTerminate;
    void threadMain();
};


#endif //NONAME_THREADPOOL_H
