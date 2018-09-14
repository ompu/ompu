#pragma once

#include "ompu/plug/game/game_fwd.hpp"
#include "ompu/midi/midi_fwd.hpp"
#include "ompu/component_id.hpp"

#include "JuceHeader.h"

#include <unordered_map>


namespace ompu { namespace plug { namespace game {

class EventHandler
    : public juce::MidiInputCallback
{
public:
    explicit EventHandler(ompu::game::EventHandler* eh);
    ~EventHandler() = default;

    virtual void handleIncomingMidiMessage(juce::MidiInput* src, juce::MidiMessage const& msg) override;

#if 0
    virtual void handlePartialSysexMessage(juce::MidiInput* src, juce::uint8 const* data, int bytes_sofar, double timestamp) override;
#endif

    void add_midi_in_dev(juce::MidiInput* dev, ComponentID id, ompu::midi::Filter filter);
    void remove_midi_in_dev(juce::MidiInput* dev);

    EventHandler(EventHandler const&) = delete;
    EventHandler(EventHandler&&) = delete;

    EventHandler& operator=(EventHandler const&) = delete;
    EventHandler& operator=(EventHandler&&) = delete;

private:
    ompu::game::EventHandler* const eh_;

    std::unordered_map<juce::MidiInput*, std::pair<ComponentID, ompu::midi::Filter>>
    midi_in_cfg_;
};

}}} // ompu
