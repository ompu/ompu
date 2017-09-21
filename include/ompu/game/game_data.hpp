#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/game/scene.hpp"
#include "ompu/midi/midi_fwd.hpp"

#include "saya/logger/logger_env.hpp"

#include <boost/lockfree/queue.hpp>

#include <chrono>
#include <memory>
#include <mutex>
#include <unordered_map>


namespace ompu { namespace game {

class GameData
{
public:
    template<OMPU_GAME_TEMPLATE_PARAMS>
    friend class Game;

    //friend class GameDataSnapshot;

    friend class EventHandler;

    friend class GC;

    // ---

    std::unique_ptr<GameDataSnapshot>
    async_snapshot();

    // ---

    GameData();
    explicit GameData(saya::logger_env_set envs);
    ~GameData() = default;

    GameData(GameData const&) = delete;
    GameData(GameData&&) = delete;

    GameData& operator=(GameData const&) = delete;
    GameData& operator=(GameData&&) = delete;

private:
    saya::logger_env_set envs_;
    scenes::all_type scene_;

public:
    saya::logger_env_set envs() const { return envs_; }
    scenes::all_type scene() const { return scene_; }
    double fps() const;

    //std::chrono::steady_clock::time_point last_snapshot_at() const { return last_snapshot_at_; }

    // ------------------------------------------

private:
    void update_fps();

    std::mutex store_mtx_;

    boost::lockfree::queue<midi::Message const*>
    in_midi_, in_midi_gc_;

    std::unordered_map<midi::Message const*, std::shared_ptr<midi::Message const>>
    in_midi_store_, in_midi_store_sub_;

#if 0
    boost::lockfree::queue<midi::Sysex const*>
    in_midi_sysex_, in_midi_sysex_gc_;

    std::unordered_map<midi::Sysex const*, std::unique_ptr<midi::Sysex>>
    in_midi_sysex_store_;
#endif

    // ------------------------------------------

private:
    std::chrono::steady_clock::time_point last_snapshot_at_, last_fps_at_;
    unsigned fps_count_{0};
    double last_fps_{0};
};

}} // ompu
