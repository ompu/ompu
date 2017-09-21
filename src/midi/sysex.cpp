#if 0

#include "ompu/midi/sysex.hpp"

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>


namespace ompu { namespace midi {

#include "ompu/def_helper.hpp"

Sysex::Sysex(
    OMPU_DEF_HELPER_EXTRACT_PARAMS(OMPU_MIDI_SYSEX_DEF)
)
OMPU_DEF_HELPER_EXTRACT_INITIALIZER_LIST(OMPU_MIDI_SYSEX_DEF)
{}

#include "ompu/def_helper_undef.hpp"

}} // ompu

#endif
