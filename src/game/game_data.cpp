#include "ompu/game/game_data.hpp"

#include "ompu/midi/message.hpp"
#include "ompu/midi/sysex.hpp"


namespace ompu { namespace game {

GameData::GameData(saya::logger_env_set envs)
    : envs_(std::move(envs))
    , in_midi_(OMPU_MIDI_IN_BUF_NUM), in_midi_gc_(OMPU_MIDI_IN_BUF_NUM * 2)

#if 0
    , in_midi_sysex_(OMPU_MIDI_IN_SYSEX_BUF_NUM), in_midi_sysex_gc_(OMPU_MIDI_IN_SYSEX_BUF_NUM * 2)
#endif

{}

GameData::GameData()
    : GameData(saya::logger_env_set{})
{}

std::unique_ptr<GameData>
GameData::async_clone()
{
    auto c = std::make_unique<GameData>();

    in_midi_.consume_all([&c] (auto const& v) {
        while (!c->in_midi_.unsynchronized_push(v)) {}
    });

#if 0
    in_midi_sysex_.consume_all([&c] (auto const& v) {
        while (!c->in_midi_sysex_.unsynchronized_push(v)) {}
    });
#endif

    return c;
}

}} // ompu
