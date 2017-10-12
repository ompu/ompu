#pragma once

#include "ompu/game/game_fwd.hpp"

#include <functional>
#include <vector>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>


namespace ompu { namespace game {

class Scheduler
{
public:
    using task_type = std::function<void()>;
    using mutex_type = std::mutex;
    using lock_type = std::unique_lock<mutex_type>;

    Scheduler() = delete;
    explicit Scheduler(std::size_t worker_count);

    Scheduler(Scheduler const&) = delete;
    Scheduler(Scheduler&&) = delete;

    ~Scheduler();

    void enqueue(task_type task);
    void halt();

private:
    void worker() noexcept;

    bool is_running_{false};
    mutex_type mtx_;
    std::condition_variable not_empty_;

    std::deque<task_type> tasks_;
    std::vector<std::thread> workers_;
};

}} // ompu
