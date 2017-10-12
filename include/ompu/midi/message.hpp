#pragma once

#include "ompu/midi/midi_fwd.hpp"
#include "ompu/component_id.hpp"

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <memory>


namespace ompu { namespace midi {

#include "ompu/def_helper.hpp"

OMPU_DEF_HELPER_EXTRACT_ENUM_CLASS(
    MessageType, OMPU_MIDI_MESSAGE_TYPE_DEF
)

class Message : public std::enable_shared_from_this<Message>
{
public:
    OMPU_DEF_HELPER_EXTRACT_CLASS_TRAITS(Message)
    OMPU_DEF_HELPER_EXTRACT_VARIABLES(OMPU_MIDI_MESSAGE_DEF)

    bool is_note_on_or_off() const noexcept { return type == MessageType::NoteOn || type == MessageType::NoteOff; }
};

#include "ompu/def_helper_undef.hpp"

}} // ompu
