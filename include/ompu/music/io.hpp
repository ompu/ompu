#pragma once

#include "ompu/music/resolver.hpp"

#include "saya/zed/seq.hpp"

#include <iosfwd>
#include <iomanip>


namespace ompu { namespace music {

namespace io_detail {

namespace tuple_print {

template<class... Tones>
inline std::ostream& operator<<(std::ostream& os, std::tuple<Tones...>)
{
    saya::zed::blackhole((os << "|" << std::setw(4) << std::left << Tones{})...);
    return os << "|";
}

} // tuple_print

template<class ScaledAs, class... Tones>
inline std::ostream& operator<<(std::ostream& os, basic_scale<ScaledAs, tone_set<Tones...>> const&)
{
    using scale_type = basic_scale<ScaledAs, tone_set<Tones...>>;

    os
        << "[Scale]\n"
        << "originally scaled as: " << ScaledAs::name << "\n"
        << "tones: "
    ;

    tuple_print::operator<<(os, saya::zed::reversed_t<std::tuple<Tones...>>{});

    os
        << "\n"
        << "canonical tones (on C): "
    ;
    tuple_print::operator<<(os, saya::zed::reversed_t<cvt::to_tuple_t<typename cvt::canonical_t<scale_type>::tone_set_type>>{});

    return os << "\n[/Scale]";
}

template<class ScaledAs, class... Scales>
inline std::ostream& operator<<(std::ostream& os, dynamic_scale<ScaledAs, Scales...> const&)
{
    using scale_type = dynamic_scale<ScaledAs, Scales...>;
    using upward_scale_type = typename scale_type::upward_scale_type;
    using downward_scale_type = typename scale_type::downward_scale_type;

    return os
        << "[Dynamic scale]\n"
        << "scaled as: " << ScaledAs::name << "\n"
        << "is_dynamic (i.e. upward != downward): " << std::boolalpha << scale_type::is_dynamic << "\n"
        << "[Upward scale]\n"
        << upward_scale_type{} << "\n[/Upward scale]\n"
        << "[Downward scale]\n"
        << downward_scale_type{} << "\n[/Downward scale]\n"
        << "[/Dynamic scale]"
    ;
}

template<class... Args>
inline std::ostream& operator<<(std::ostream& os, saya::zed::maybe_empty_seq<Args...>)
{
    saya::zed::blackhole((os << Args{})...);
    return os;
}

} // io_detail


template<class Identity, class Enabled = std::enable_if_t<false, int>>
std::ostream& operator<<(std::ostream& os, Identity const&);

template<class Ident>
inline std::ostream& operator<<(std::ostream& os, basic_tone<Ident> const&)
{
    using tone_type = basic_tone<Ident>;
    return os << tone_type::name;
}

template<class Scale, std::enable_if_t<is_scale_v<Scale>, int> = 0>
inline std::ostream& operator<<(std::ostream& os, Scale const& v)
{
    return io_detail::operator<<(os, v);
}

template<class Ident, class KeyFeel>
inline std::ostream& operator<<(std::ostream& os, basic_key<Ident, KeyFeel> const&)
{
    using key_type = basic_key<Ident, KeyFeel>;

    return os
        << "[Key]\n"
        << "name: " << key_type::name << "\n"
        << "[Key scale]\n" << typename key_type::key_scale_type{} << "\n[/Key scale]\n"
        << "[/Key]"
        //<< ompu::music::make_resolve_in_key(key_type::key_scale_type{}, key_type{})
    ;
}

}} // ompu
