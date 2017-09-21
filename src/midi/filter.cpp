#include "ompu/midi/filter.hpp"
#include "ompu/midi/message.hpp"


namespace ompu { namespace midi {

bool Filter::is_filtered(Message const& msg) const noexcept
{
    if (channel[msg.ch - 1]) {
        return true;
    }

    return false;
}

}} // ompu
