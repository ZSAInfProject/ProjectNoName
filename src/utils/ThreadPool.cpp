#include "ThreadPool.h"
#include "Log.h"

void ThreadPool::threadMain() {
    std::function<void (void)> job;
    while(true){
        {
            std::unique_lock<std::mutex> lock(jobsMut);
            while (jobs.empty() && !shouldTerminate)
                cond.wait(lock);

            if(jobs.empty()){
                Log::verbose(TAG, "Thread terminating...");
                return;
            }

            job = std::move(jobs.front());
            jobs.pop();
        }
        job();
        scheduledJobs--;
    }

}

void ThreadPool::addJob(std::function<void(void)> job) {
    std::unique_lock <std::mutex> lock(jobsMut);
    jobs.emplace(std::move(job));
    scheduledJobs++;
    cond.notify_one();
}

ThreadPool::ThreadPool(int numberOfThreads) {
    shouldTerminate = false;
    for(int i = 0; i < numberOfThreads; i++){
        threads.emplace_back([this](){threadMain();});
    }
    Log::debug(TAG, "Spawned " + std::to_string(numberOfThreads) + " threads");
}

ThreadPool::~ThreadPool() {
    shouldTerminate = true;
    Log::debug(TAG, "Terminating...");
    cond.notify_all();
    for(auto& thread: threads)
        thread.join();
}

bool ThreadPool::finished() {
    return scheduledJobs.load() == 0;
}
