#include "ompu/midi/message.hpp"

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>

#include <iostream>


namespace ompu { namespace midi {

#include "ompu/def_helper.hpp"

OMPU_DEF_HELPER_EXTRACT_ENUM_CLASS_IMPL(
    MessageType, OMPU_MIDI_MESSAGE_TYPE_DEF
)

#include "ompu/def_helper_undef.hpp"

}} // ompu
