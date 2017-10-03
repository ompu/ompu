#pragma once

#include "ompu/music.hpp"
#include "ompu/music/io.hpp"

#include <boost/format.hpp>

#include <iosfwd>


namespace ompu { namespace music {

namespace detail {

template<std::size_t... Heights>
inline void debug_all_impl(std::ostream& os, std::index_sequence<Heights...>)
{
    static std::string const SEP = "\n--------------------------------------\n";

    os << SEP << "ompu::music debug output (sampled)" << SEP << std::endl;

    os
        << SEP << "Major keys" << SEP
        << music::keys::CMaj{} << std::endl
    ;
}

inline void debug_all(std::ostream& os)
{
    return debug_all_impl(os, std::make_index_sequence<12>{});
}

} // detail

}} // ompu
