#include "ompu/game/gc.hpp"
#include "ompu/game/game_data.hpp"
#include "ompu/midi.hpp"

#include "ompu/util.hpp"

#include <boost/scope_exit.hpp>

#include <thread>
#include <chrono>

#include <ctime>


namespace ompu { namespace game {

GC::GC(GameData* const gd)
    : gd_(gd)
    , l_(gd->envs(), "GC")
    , is_running_{false}
{}

void GC::start()
{
    l_.info() << "start" << std::endl;
    is_running_ = true;

    try {
        while (is_running_) {
            if (this->need_run()) {
                this->run();
            }

            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        //
        // ensure everything is cleaned
        //

        // if (this->need_run()) {
            this->run();
        // }

    } catch (std::exception const& e) {
        l_.note() << e.what() << std::endl;
        l_.error() << "!!! [BUG] exception thrown in GC !!!" << std::endl;
        throw;

    } catch (...) {
        l_.error() << "!!! [BUG] unknown exception thrown in GC !!!" << std::endl;
        throw;
    }
}

void GC::stop()
{
    l_.info() << "stopping..." << std::endl;
    is_running_ = false;
}

bool GC::need_run()
{
    if (
        !gd_->in_midi_gc_.empty()

#if 0
        || !gd_->in_midi_sysex_gc_.empty()
#endif

    ) {
        return true;
    }

    return false;
}

void GC::run()
{
    auto const curr_run_start = clock_type::now();
    std::size_t cleaned_obj_count = 0;

    BOOST_SCOPE_EXIT_ALL(this, curr_run_start) {
        this->prof_->last_run_at_steady = curr_run_start;
    };

    {
        auto const now = std::chrono::system_clock::now();
        l_.note() << "this run: " << ompu::util::time_print{now} << std::endl;

        if (prof_) {
            l_.note() << "last run: " << ompu::util::time_print{prof_->last_run_at} << std::endl;
            l_.note() << "last run was " << std::chrono::duration_cast<std::chrono::seconds>(now - prof_->last_run_at).count() << " sec ago" << std::endl;

            prof_ = std::make_unique<Profile>(*prof_);

        } else {
            l_.note() << "last run: (none, this is the initial run)" << std::endl;
            prof_ = std::make_unique<Profile>();
        }

        l_.info() << "running..." << std::endl;
    }

    {
        // do the GC
        std::lock_guard<decltype(gd_->store_mtx_)>
        lock(gd_->store_mtx_);

        cleaned_obj_count += gd_->in_midi_gc_.consume_all([this] (auto const& p) {
            gd_->in_midi_store_.erase(p);
        });
    }

    auto const curr_run_end = clock_type::now();

    l_.note()
        << "cleaned objects count: "
        << cleaned_obj_count
        << std::endl
    ;
    l_.note()
        << "took: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(
            curr_run_end - curr_run_start
           ).count()
        << " msec"
        << std::endl
    ;
    l_.info() << "done" << std::endl;
}

}} // ompu
