#include "ompu/midi/message.hpp"
#include "ompu/midi/sysex.hpp"
#include "ompu/midi/track.hpp"
#include "ompu/midi/io.hpp"

#include <boost/core/ignore_unused.hpp>

#include <iostream>


namespace ompu { namespace midi {

std::ostream& operator<<(std::ostream& os, Message const& v)
{
    boost::ignore_unused(v);
    return os << "TODO: implement (<<) midi::Message";
}

#if 0
std::ostream& operator<<(std::ostream& os, Sysex const& v)
{
    boost::ignore_unused(v);
    return os << "TODO: implement (<<) midi::Sysex";
}
#endif

std::ostream& operator<<(std::ostream& os, Track const& v)
{
    boost::ignore_unused(v);
    return os << "TODO: implement (<<) midi::Track";
}

}} // ompu

