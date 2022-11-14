#pragma once

#include <cassert>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace fibers {

template <typename T>
class UnboundedBlockingQueue {
public:
    bool put(T value) {
        std::lock_guard guard(m_mutex);
        if (m_closed) {
            return false;
        }
        m_buffer.push_back(std::move(value));
        m_notEmpty.notify_one();
        return true;
    }

    std::optional<T> take() {
        std::unique_lock lock(m_mutex);
        while (m_buffer.empty()) {
            if (m_closed) {
                return std::nullopt;
            }
            m_notEmpty.wait(lock);
        }
        return takeLocked();
    }

    void close() {
        closeImpl(/*clear=*/false);
    }

    void cancel() {
        closeImpl(/*clear=*/true);
    }

private:
    T takeLocked() {
        assert(!m_buffer.empty());

        T front = std::move(m_buffer.front());
        m_buffer.pop_front();
        return front;
    }

    void closeImpl(bool clear) {
        std::lock_guard guard(m_mutex);
        m_closed = true;
        if (clear) {
            m_buffer.clear();
        }
        m_notEmpty.notify_all();
    }

private:
    std::deque<T> m_buffer;
    bool m_closed = false;

    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
};

}  // namespace fibers
