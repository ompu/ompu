#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/game/error.hpp"
#include "ompu/game/scene.hpp"
#include "ompu/game/game_data.hpp"
#include "ompu/game/visitor.hpp"
#include "ompu/game/event_handler.hpp"
#include "ompu/game/gc.hpp"

#include "saya/logger.hpp"

#include <boost/scope_exit.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <memory>
#include <chrono>
#include <thread>
#include <atomic>


namespace ompu { namespace game {

template<
    //
    // see "ompu/game/visitor.hpp" for possible implementation
    //

    OMPU_GAME_TEMPLATE_PARAMS
>
class Game
{
public:
    explicit Game(saya::logger_env_set envs, SceneUpdateVisitor* const suv, SceneDrawVisitor* const sdv)
        : is_running_(false)
        , gd_(std::make_unique<GameData>(std::move(envs)))
        , l_(gd_->envs(), "Game")
        , eh_(gd_.get())
        , gc_(gd_.get())
        , suv_(suv)
        , sdv_(sdv)
    {
        l_.note() << "using update visitor from '" << SceneUpdateVisitor::name() << "'" << std::endl;
        l_.note() << "using draw visitor from '" << SceneDrawVisitor::name() << "'" << std::endl;
        l_.info() << "initialized." << std::endl;

        gc_thread_ = std::thread([this] {
            gc_.start();
        });
    }

    ~Game()
    {
        //l_.info() << "game over" << std::endl;

        //l_.info() << "stopping GC..." << std::endl;
        gc_.stop();

        if (gc_thread_.joinable()) {
            gc_thread_.join();
        }

        //l_.info() << "bye" << std::endl;
    }

#if 0
    void run()
    {
        while (is_running_) {
            this->synced_frame();

            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        gc_.stop();
    }
#endif

    void start() { /* gc_.start(); */ is_running_ = true; }
    void stop() { is_running_ = false; gc_.stop(); }

    void synced_frame()
    {
        if (!is_running_) {
            throw std::logic_error("Game::synced_frame() called without running game instance");
        }

        auto gd_ss = gd_->async_snapshot();

        // mark for GC at first
        for (auto& msg : gd_ss->midi_msgs) {
            while (!gd_->in_midi_gc_.push(msg.get())) {}
        }

        // (1) update
        auto const next_scene = this->update(gd_ss.get());

        // (1.1) hook scene change
        BOOST_SCOPE_EXIT_ALL(this, next_scene)
        {
            if (gd_->scene_.which() != next_scene.which()) {
                l_.note() << "prev:  '" << boost::apply_visitor(SceneNameVisitor{}, gd_->scene_) << "'" << std::endl;
                l_.note() << "after: '" << boost::apply_visitor(SceneNameVisitor{}, next_scene) << "'" << std::endl;
                l_.info() << "scene changed" << std::endl;
            }

            gd_->scene_ = next_scene;
        };

        // (2) draw
        this->draw(std::move(gd_ss));
    }

    Game(Game const&) = delete;
    Game(Game&&) = delete;

    Game& operator=(Game const&) = delete;
    Game& operator=(Game&&) = delete;

    EventHandler* const eh() noexcept { return &eh_; }

private:
    update_visitor_return_type
    update(update_visitor_game_data_type gd)
    {
        auto const current_scene = gd->scene;

        try {
            suv_->game_data(std::move(gd));
            return boost::apply_visitor(*suv_, current_scene);

        } catch (game_error const& e) {
            l_.error() << e.what() << std::endl;
        }

        return current_scene;
    }

    draw_visitor_return_type
    draw(draw_visitor_game_data_type gd)
    {
        try {
            auto const current_scene = gd->scene;
            sdv_->game_data(std::move(gd));
            return boost::apply_visitor(*sdv_, current_scene);

        } catch (game_error const& e) {
            l_.error() << e.what() << std::endl;
        }
    }


    std::atomic<bool> is_running_;
    std::unique_ptr<GameData> gd_;

    SceneUpdateVisitor* const suv_;
    SceneDrawVisitor* const sdv_;

    saya::logger l_;

    EventHandler eh_;

    GC gc_;
    std::thread gc_thread_;
};

}} // ompu
