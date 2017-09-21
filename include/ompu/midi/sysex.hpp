#pragma once

#if 0

#include "ompu/midi/midi_fwd.hpp"
#include "ompu/midi/message.hpp"

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>


namespace ompu { namespace midi {

#include "ompu/def_helper.hpp"

class Sysex
{
public:
    explicit Sysex(
        OMPU_DEF_HELPER_EXTRACT_PARAMS(OMPU_MIDI_SYSEX_DEF)
    );

    OMPU_DEF_HELPER_EXTRACT_CLASS_TRAITS(Sysex)

    OMPU_DEF_HELPER_EXTRACT_VARIABLES(OMPU_MIDI_SYSEX_DEF)
};

#include "ompu/def_helper_undef.hpp"

}} // ompu

#endif
