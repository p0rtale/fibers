#include <fibers/thread_pool/ThreadPool.hpp>

#include <iostream>
#include <thread>

namespace fibers {

thread_local static ThreadPool* pool;

ThreadPool::ThreadPool(size_t workers) {
    startWorkers(workers);
}

ThreadPool::~ThreadPool() {
    bool stopped = std::none_of(m_workers.cbegin(), m_workers.cend(), 
                                std::bind(&std::thread::joinable, std::placeholders::_1));
    assert(stopped);
}

void ThreadPool::addTask(Task task) {
    m_waitGroup.add();
    m_tasks.put(std::move(task));
}

void ThreadPool::wait() {
    m_waitGroup.wait();
}

void ThreadPool::stop() {
    m_tasks.cancel();
    for (auto& worker: m_workers) {
        worker.join();
    }
}

void ThreadPool::startWorkers(size_t count) {
    for (size_t i = 0; i < count; ++i) {
        m_workers.emplace_back([this](){
            pool = this;
            workerRoutine();
        });
    }
}

void ThreadPool::workerRoutine() {
    while (true) {
        auto task = m_tasks.take();
        if (!task) {
            break;
        }

        try {
            (*task)();
        } catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        m_waitGroup.done();
    }
}

ThreadPool* ThreadPool::current() {
    return pool;
}

}  // namespace fibers
