#include "ThreadPool.h"
#include "Log.h"

void ThreadPool::threadMain() {
    std::function<void(void)> job;
    while(true) {
        {
            std::unique_lock<std::mutex> lock(jobsMut);
            while(jobs.empty() && !shouldTerminate)
                jobsCV.wait(lock);

            if(jobs.empty()) {
                Log::verbose(TAG, "Thread terminating...");
                return;
            }

            job = std::move(jobs.front());
            jobs.pop();
        }
        job();
        std::unique_lock<std::mutex> lock(scheduledJobsMut);
        scheduledJobs--;
        scheduledJobsCV.notify_all();
    }

}

void ThreadPool::addJob(std::function<void(void)> job) {
    std::unique_lock<std::mutex> lock(jobsMut);
    jobs.emplace(std::move(job));
    std::unique_lock<std::mutex> lock2(scheduledJobsMut);
    scheduledJobs++;
    jobsCV.notify_one();
}

ThreadPool::ThreadPool(int numberOfThreads) {
    scheduledJobs = 0;
    shouldTerminate = false;
    for(int i = 0; i < numberOfThreads; i++) {
        threads.emplace_back([this]() { threadMain(); });
    }
    Log::debug(TAG, "Spawned " + std::to_string(numberOfThreads) + " threads");
}

ThreadPool::~ThreadPool() {
    shouldTerminate = true;
    Log::debug(TAG, "Terminating...");
    jobsCV.notify_all();
    for(auto& thread: threads)
        thread.join();
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(scheduledJobsMut);
    while(scheduledJobs > 0) {
        scheduledJobsCV.wait(lock);
    }
}
