#include "ompu/game/scheduler.hpp"


namespace ompu { namespace game {

Scheduler::Scheduler(std::size_t worker_count)
    : is_running_(true)
{
    for (std::size_t i = 0; i < worker_count; ++i) {
        workers_.emplace_back([=] { this->worker(); });
    }
}

Scheduler::~Scheduler()
{
    for (auto& w : workers_) {
        if (w.joinable()) {
            w.join();
        }
    }
}

void Scheduler::worker() noexcept
{
    while (true) {
        lock_type lock(mtx_);
        not_empty_.wait(lock, [this] {
            return !tasks_.empty() || !is_running_;
        });
        if (!is_running_) return;

        auto task = std::move(tasks_.front());
        tasks_.pop_front();
        lock.unlock();

        task();
    }
}

void Scheduler::enqueue(task_type task)
{
    lock_type lock(mtx_);
    tasks_.push_back(std::move(task));
    not_empty_.notify_all();
}

void Scheduler::halt()
{
    lock_type lock(mtx_);
    is_running_ = false;
    not_empty_.notify_all();
}

}} // ompu
