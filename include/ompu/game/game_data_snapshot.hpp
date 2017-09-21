#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/game/scene.hpp"
#include "ompu/midi.hpp"

#include <vector>
#include <chrono>


namespace ompu { namespace game {

class GameDataSnapshot
{
public:
    GameDataSnapshot() = delete;
    explicit GameDataSnapshot(GameData const* gd);

    ~GameDataSnapshot() = default;

    GameDataSnapshot(GameDataSnapshot const&) = delete;
    GameDataSnapshot(GameDataSnapshot&&) noexcept = default;

    GameDataSnapshot& operator=(GameDataSnapshot const&) = delete;
    GameDataSnapshot& operator=(GameDataSnapshot&&) noexcept = default;

    scenes::all_type scene;

    std::chrono::steady_clock::time_point snapshot_at;
    double fps;

    std::vector<std::shared_ptr<midi::Message const>>
    midi_msgs;
};

}} // ompu
