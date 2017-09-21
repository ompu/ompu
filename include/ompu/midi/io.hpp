#pragma once

#include "ompu/midi/midi_fwd.hpp"

#include <iosfwd>

namespace ompu { namespace midi {

std::ostream& operator<<(std::ostream& os, Message const& v);

#if 0
std::ostream& operator<<(std::ostream& os, Sysex const& v);
#endif

std::ostream& operator<<(std::ostream& os, Track const& v);

}} // ompu
