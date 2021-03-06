#include "ompu/game/gc.hpp"
#include "ompu/game/game_data.hpp"
#include "ompu/midi.hpp"

#include "ompu/util.hpp"

#include <boost/scope_exit.hpp>

#include <thread>
#include <chrono>

#include <ctime>


namespace ompu { namespace game {

GC::Profile::Profile()
    : last_run_at(std::chrono::system_clock::now())
{
}

GC::GC(GameData* const gd)
    : gd_(gd)
    , l_(gd->envs(), "GC")
    , is_running_{false}
{}

GC::~GC()
{
    l_.note()
        << "remaining objects: "
        << gd_->in_midi_store_.size() + gd_->in_midi_store_sub_.size()
        << std::endl
    ;
    l_.note() << "(remaining objects will be destructed by the runtime)" << std::endl;
    l_.info() << "terminating..." << std::endl;
}

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

#if 0
        //
        // ensure everything is cleaned
        //

        // if (this->need_run()) {
            this->run();
        // }
#endif

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
    //l_.info() << "stopping..." << std::endl;
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
    auto const curr_run_at = std::chrono::system_clock::now();
    std::size_t cleaned_obj_count = 0;

    BOOST_SCOPE_EXIT_ALL(this, curr_run_at, curr_run_start) {
        this->prof_->last_run_at = curr_run_at;
        this->prof_->last_run_at_steady = curr_run_start;
    };

    {
        l_.note() << "this run: " << ompu::util::time_print{curr_run_at} << std::endl;

        if (prof_) {
            l_.note() << "last run: " << ompu::util::time_print{prof_->last_run_at} << std::endl;
            l_.note() << "last run was " << std::chrono::duration_cast<std::chrono::seconds>(curr_run_at - prof_->last_run_at).count() << " sec ago" << std::endl;

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
