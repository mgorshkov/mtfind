#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <tuple>
#include <vector>

#include "IMatcher.hpp"
#include "IStatCollector.hpp"
#include "Using.hpp"

class ThreadedMatcher : public IMatcher {
public:
    ThreadedMatcher(IStatCollector *statCollector, unsigned int nThreads, Mask mask);

    ~ThreadedMatcher();

    void match(BufferInfo &&bufferInfo) override;

private:
    void threadProc();
    bool checkWatermark();
    void finalizePool();

    static const size_t MAX_QUEUE_SIZE = 100;

    IStatCollector *m_statCollector;
    Mask m_mask;

    std::queue<BufferInfo> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::vector<std::thread> m_threadPool;
    std::atomic_bool m_finalize;
};