//
// Created by Mikhail Gorshkov on 06.07.2022.
//

#include "ThreadedMatcher.hpp"

#include "Matcher.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <utility>

using namespace std::chrono_literals;

#ifdef __gnu_linux__
static int pin_to_cpu(pthread_t thread_handle, int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    return pthread_setaffinity_np(thread_handle, sizeof(cpuset), &cpuset);
}
#endif

ThreadedMatcher::ThreadedMatcher(IStatCollector *statCollector, unsigned int nThreads, Mask mask)
    : m_statCollector(statCollector), m_mask{std::move(mask)} {
    m_finalize.store(false, std::memory_order_relaxed);
    for (int i = 0; i < nThreads; ++i) {
        m_threadPool.emplace_back(&ThreadedMatcher::threadProc, this);
#ifdef __gnu_linux__
        const int cpu = i % processor_count;
        int res = pin_to_cpu(m_threadPool[i].native_handle(), cpu);
        assert(res == 0);
#endif
    }
}

ThreadedMatcher::~ThreadedMatcher() {
    finalizePool();
}

void ThreadedMatcher::match(BufferInfo &&bufferInfo) {
    while (checkWatermark()) {// backpressure the producer if the consumer doesn't keep up
        std::this_thread::sleep_for(500ms);
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(bufferInfo));
    }

    m_condition.notify_one();
}

bool ThreadedMatcher::checkWatermark() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size() >= MAX_QUEUE_SIZE;
}

void ThreadedMatcher::threadProc() {
    try {
        Matcher matcher(m_statCollector, m_mask);
        while (true) {
            auto work = [this]() -> BufferInfo {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_condition.wait(lock, [this]() {
                    return !m_queue.empty() || m_finalize.load();
                });
                if (m_queue.empty())
                    return std::move(BufferInfo{nullptr, 0, 0});

                BufferInfo info = std::move(m_queue.front());
                m_queue.pop();
                return std::move(info);
            };
            auto bufferInfo = work();
            if (bufferInfo.buffer == nullptr)
                break;
            matcher.match(std::move(bufferInfo));
        }
    } catch (...) {
        std::cerr << "Exception caught in threadProc" << std::endl;
    }
}

void ThreadedMatcher::finalizePool() {
    m_finalize.store(true);
    m_condition.notify_all();
    for (auto &thread: m_threadPool)
        thread.join();
}