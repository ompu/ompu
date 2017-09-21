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
    explicit Game(saya::logger_env_set envs)
        : is_running_(false)
        , data_(std::make_unique<GameData>(std::move(envs)))
        , l_(data_->envs(), "Game")
        , eh_(data_.get())
        , scene_(scenes::Noop{})
        , gc_(data_.get())
    {
        l_.note() << "using update visitor: " << SceneUpdateVisitor::name() << std::endl;
        l_.note() << "using draw visitor: " << SceneDrawVisitor::name() << std::endl;
        l_.info() << "initialized." << std::endl;

        gc_thread_ = std::thread([this] {
            gc_.start();
        });
    }

    ~Game()
    {
        l_.info() << "game over" << std::endl;

        l_.info() << "stopping GC..." << std::endl;
        gc_.stop();

        if (gc_thread_.joinable()) {
            gc_thread_.join();
        }

        l_.info() << "bye" << std::endl;
    }

    void run()
    {
        while (is_running_) {
            this->frame();

            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        gc_.stop();
    }

    void stop() { is_running_ = false; }


    Game(Game const&) = delete;
    Game(Game&&) = delete;

    Game& operator=(Game const&) = delete;
    Game& operator=(Game&&) = delete;

    EventHandler* const eh() noexcept { return &eh_; }

private:
    void frame()
    {
        auto data_copy = this->data_->async_clone();
        this->update(data_copy.get());
        this->draw(data_copy.get());

        data_copy->in_midi_.consume_all([this] (auto const& p) {
            while (!data_->in_midi_gc_.push(p)) {}
        });
    }

    void update(GameData* const gd)
    {
        scenes::all_type next_scene{scenes::Noop{}};

        BOOST_SCOPE_EXIT_ALL(this, &next_scene)
        {
            auto const scene_before = scene_;

            scene_ = std::move(next_scene);

            if (scene_.which() != scene_before.which()) {
                l_.note() << "before: '" << boost::apply_visitor(SceneNameVisitor{}, scene_before) << "'" << std::endl;
                l_.note() << "after: '" << boost::apply_visitor(SceneNameVisitor{}, scene_) << "'" << std::endl;
                l_.info() << "scene changed" << std::endl;
            }
        };

        try {
            SceneUpdateVisitor suv{gd};
            next_scene = boost::apply_visitor(suv, scene_);

        } catch (game_error const& e) {
            l_.error() << e.what() << std::endl;
        }
    }

    void draw(GameData const* const gd) const
    {
        try {
            SceneDrawVisitor const sdv{gd};
            boost::apply_visitor(sdv, scene_);

        } catch (game_error const& e) {
            l_.error() << e.what() << std::endl;
        }
    }


    std::atomic<bool> is_running_;
    std::unique_ptr<GameData> data_;

    saya::logger l_;

    EventHandler eh_;

    scenes::all_type scene_;

    GC gc_;
    std::thread gc_thread_;
};

}} // ompu
