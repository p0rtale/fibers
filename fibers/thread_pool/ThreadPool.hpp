#pragma once

#include <fibers/thread_pool/Task.hpp>
#include <fibers/thread_pool/UnboundedBlockingQueue.hpp>
#include <fibers/thread_pool/WaitGroup.hpp>

#include <thread>
#include <vector>

namespace fibers {

class ThreadPool {
public:
    explicit ThreadPool(size_t workers);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void addTask(Task task);

    void wait();

    void stop();

    static ThreadPool* current();

private:
    void startWorkers(size_t count);

    void workerRoutine();

private:
    std::vector<std::thread> m_workers;
    UnboundedBlockingQueue<Task> m_tasks;
    WaitGroup m_waitGroup;
};

inline ThreadPool* currentThreadPool() {
    return ThreadPool::current();
}

}  // namespace fibers
