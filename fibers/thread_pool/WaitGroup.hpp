#pragma once

#include <atomic>
#include <condition_variable>
#include <cassert>
#include <mutex>

namespace fibers {

class WaitGroup {
public:
    void add() {
        std::lock_guard guard(m_mutex);
        ++m_counter;
    }

    void done() {
        assert(m_counter > 0);

        --m_counter;
        if (m_counter == 0) {
            std::lock_guard guard(m_mutex);
            m_allDone.notify_all();
        }
    }

    void wait() {
        std::unique_lock lock(m_mutex);
        while (m_counter != 0) {
            m_allDone.wait(lock);
        }
    }

private:
    std::atomic<size_t> m_counter = {0};
    std::mutex m_mutex;
    std::condition_variable m_allDone;
};

}  // namespace fibers
