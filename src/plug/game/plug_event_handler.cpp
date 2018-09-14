#include "ompu/game/event_handler.hpp"
#include "ompu/plug/game/event_handler.hpp"

#include "ompu/midi/message.hpp"
#include "ompu/midi/sysex.hpp"
#include "ompu/midi/filter.hpp"

#include "ompu/game/game_data.hpp"

#include <boost/typeof/typeof.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/assert.hpp>


namespace ompu { namespace plug { namespace game {

EventHandler::EventHandler(ompu::game::EventHandler* const eh)
    : eh_(eh)
{}

void EventHandler::add_midi_in_dev(juce::MidiInput* const dev, ComponentID id, ompu::midi::Filter filter)
{
    BOOST_ASSERT(!midi_in_cfg_.count(dev));
    midi_in_cfg_[dev] = std::make_pair(std::move(id), std::move(filter));
}

void EventHandler::remove_midi_in_dev(juce::MidiInput* const dev)
{
    BOOST_ASSERT(midi_in_cfg_.count(dev));
    midi_in_cfg_.erase(dev);
}

namespace {

std::unique_ptr<midi::Message>
make_midi(midi::MessageType t, ComponentID id, juce::MidiMessage const& msg)
{
    auto ret = std::make_unique<midi::Message>();
    ret->type = std::move(t);
    ret->src_id = std::move(id);
    ret->ch = static_cast<BOOST_TYPEOF(midi::Message::ch)>(msg.getChannel());

    switch (ret->type) {
    case midi::MessageType::NoteOn:
    case midi::MessageType::NoteOff:
        ret->note_num = static_cast<BOOST_TYPEOF(midi::Message::note_num)>(msg.getNoteNumber());
        break;
    }

    return ret;
}

} // anon

void EventHandler::handleIncomingMidiMessage(juce::MidiInput* const dev, juce::MidiMessage const& j_msg)
{
    BOOST_ASSERT(midi_in_cfg_.count(dev));
    auto const& cfg = midi_in_cfg_.at(dev);

    midi::MessageType t;
    if (j_msg.isNoteOn()) {
        t = midi::MessageType::NoteOn;
    } else if (j_msg.isNoteOff()) {
        t = midi::MessageType::NoteOff;
    }

    auto msg = make_midi(
        std::move(t),
        cfg.first,
        j_msg
    );

    std::cout << j_msg.getDescription() << std::endl;

    if (cfg.second.is_filtered(*msg)) return;
    eh_->in_midi(std::move(msg));
}

#if 0
void EventHandler::handlePartialSysexMessage(juce::MidiInput* const dev, juce::uint8 const* data, int bytes_sofar, double timestamp)
{
    // FIXME
    boost::ignore_unused(data, bytes_sofar, timestamp);

    eh_->in_midi_sysex(std::make_unique<midi::Sysex>(
        midi::Message{
            midi_in_cfg_.at(dev)
        }
        //, msg
    ));
}
#endif

}}} // ompu
