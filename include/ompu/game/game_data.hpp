#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/midi/midi_fwd.hpp"

#include "saya/logger/logger_env.hpp"

#include <boost/lockfree/queue.hpp>

#include <memory>
#include <mutex>
#include <unordered_map>


namespace ompu { namespace game {

class GameData
{
public:
    template<OMPU_GAME_TEMPLATE_PARAMS>
    friend class Game;

    friend class GC;

    // ---

    std::unique_ptr<GameData> async_clone();

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

public:
    saya::logger_env_set envs() const { return envs_; }

    // ------------------------------------------

    std::mutex store_mtx_;

    boost::lockfree::queue<midi::Message const*>
    in_midi_, in_midi_gc_;

    std::unordered_map<midi::Message const*, std::unique_ptr<midi::Message>>
    in_midi_store_, in_midi_store_sub_;

#if 0
    boost::lockfree::queue<midi::Sysex const*>
    in_midi_sysex_, in_midi_sysex_gc_;

    std::unordered_map<midi::Sysex const*, std::unique_ptr<midi::Sysex>>
    in_midi_sysex_store_;
#endif

    // ------------------------------------------
};

}} // ompu
