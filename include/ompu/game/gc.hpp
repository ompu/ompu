#pragma once
#include "ompu/game/game_fwd.hpp"

#include "saya/logger.hpp"

#include <iosfwd>
#include <memory>
#include <atomic>
#include <chrono>


namespace ompu { namespace game {

class GC
{
public:
    explicit GC(GameData* gd);
    ~GC() = default;

    void start();
    void stop();

    GC(GC const&) = delete;
    GC(GC&&) = delete;

    GC& operator=(GC const&) = delete;
    GC& operator=(GC&&) = delete;

private:
    bool need_run();
    void run();

    GameData* const gd_;
    saya::logger l_;

    std::atomic<bool> is_running_;

    // profiles ----------------------
    using clock_type = std::chrono::steady_clock;

    struct Profile
    {
        std::chrono::system_clock::time_point
        last_run_at;

        std::chrono::steady_clock::time_point
        last_run_at_steady;
    };

    std::unique_ptr<Profile> prof_;
};

}} // ompu
