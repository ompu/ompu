#include "ompu/game/game_data.hpp"
#include "ompu/game/scene.hpp"
#include "ompu/game/game_data_snapshot.hpp"

#include "ompu/midi/message.hpp"
#include "ompu/midi/sysex.hpp"


namespace ompu { namespace game {

GameData::GameData(saya::logger_env_set envs)
    : envs_(std::move(envs))
    , scene_(scenes::Noop{})
    , in_midi_(OMPU_MIDI_IN_BUF_NUM), in_midi_gc_(OMPU_MIDI_IN_BUF_NUM * 2)
    , last_snapshot_at_(std::chrono::steady_clock::now())
    , last_fps_at_(last_snapshot_at_)

#if 0
    , in_midi_sysex_(OMPU_MIDI_IN_SYSEX_BUF_NUM), in_midi_sysex_gc_(OMPU_MIDI_IN_SYSEX_BUF_NUM * 2)
#endif

{}

GameData::GameData()
    : GameData(saya::logger_env_set{})
{}

void GameData::update_fps()
{
    ++fps_count_;

    auto const now = std::chrono::steady_clock::now();
    auto const delta = now - last_fps_at_;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(delta) > std::chrono::milliseconds(1000)) {
        last_fps_at_ = now;
        last_fps_ =
            fps_count_ / (double(delta.count()) / 1000.)
        ;

        fps_count_ = 0;
    }
}

double GameData::fps() const
{
    return last_fps_;
}

std::shared_ptr<GameDataSnapshot>
GameData::async_snapshot()
{
    auto ss = std::make_shared<GameDataSnapshot>(this);

    in_midi_.consume_all([&ss] (auto& v) {
        ss->midi_msgs.emplace_back(v->shared_from_this());
    });

#if 0
    in_midi_sysex_.consume_all([&c] (auto const& v) {
        while (!c->in_midi_sysex_.unsynchronized_push(v)) {}
    });
#endif

    return ss;
}

}} // ompu
