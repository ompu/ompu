#pragma once

#include "ompu/midi/midi_fwd.hpp"
#include "ompu/component_id.hpp"

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>


namespace ompu { namespace midi {

#include "ompu/def_helper.hpp"

class Message
{
public:
    explicit Message(
        OMPU_DEF_HELPER_EXTRACT_PARAMS(OMPU_MIDI_MESSAGE_DEF)
    );

    OMPU_DEF_HELPER_EXTRACT_CLASS_TRAITS(Message)

    OMPU_DEF_HELPER_EXTRACT_VARIABLES(OMPU_MIDI_MESSAGE_DEF)
};

#include "ompu/def_helper_undef.hpp"

}} // ompu
