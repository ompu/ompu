#pragma once

#include "ompu/music/resolver.hpp"

#include "saya/zed/seq.hpp"

#include <iosfwd>
#include <iomanip>


namespace ompu { namespace music {

namespace io_detail {

template<class... Tones, class... Mods>
inline std::ostream& operator<<(std::ostream& os, std::tuple<basic_ident<Tones, Mods>...>)
{
    saya::zed::blackhole((os << "|" << std::setw(4) << std::left << basic_ident<Tones, Mods>{})...);
    return os << "|";
}

template<class... FundSet, class... TensionSet>
inline std::ostream& operator<<(std::ostream& os, std::tuple<basic_chord<FundSet, TensionSet>...>)
{
    saya::zed::blackhole((os << "|" << std::setw(6) << std::left << basic_chord<FundSet, TensionSet>{})...);
    return os << "|";
}

template<class FundSet, class TensionSet>
inline std::ostream& operator<<(std::ostream& os, basic_chord<FundSet, TensionSet> const&)
{
    using chord_type = basic_chord<FundSet, TensionSet>;
    return os
        << "(chord)"
    ;
}

template<class ScaledAs, class... Idents>
inline std::ostream& operator<<(std::ostream& os, basic_scale<ScaledAs, ident_set<Idents...>> const&)
{
    using scale_type = basic_scale<ScaledAs, ident_set<Idents...>>;

    return os
        << "[Scale]\n"
        << "originally scaled as: " << ScaledAs::name << "\n"
        << "tones: " << saya::zed::reversed_t<std::tuple<Idents...>>{} << "\n"
        << "canonical tones (on C): "
        << saya::zed::reversed_t<cvt::to_tuple_t<typename cvt::canonical_t<scale_type>::ident_set_type>>{}
        << "\n[/Scale]"
    ;
}

template<class ScaledAs, class... Scales>
inline std::ostream& operator<<(std::ostream& os, dynamic_scale<ScaledAs, Scales...> const&)
{
    using scale_type = dynamic_scale<ScaledAs, Scales...>;
    using upward_scale_type = typename scale_type::upward_scale_type;
    using downward_scale_type = typename scale_type::downward_scale_type;

    os
        << "[Dynamic scale]\n"
        << "scaled as: " << ScaledAs::name << "\n"
        << "is_dynamic (i.e. upward != downward): " << std::boolalpha << scale_type::is_dynamic << "\n"
    ;
    if (scale_type::is_dynamic) {
        os
            << "[Upward scale]\n"
            << upward_scale_type{} << "\n[/Upward scale]\n"
            << "[Downward scale]\n"
            << downward_scale_type{} << "\n[/Downward scale]\n"
        ;
    } else {
        os
            << upward_scale_type{} << "\n"
        ;
    }
    return os << "[/Dynamic scale]";
}

} // io_detail


template<class Identity, class Enabled = std::enable_if_t<false, int>>
std::ostream& operator<<(std::ostream& os, Identity const&);

template<class Tone, class Mod>
inline std::ostream& operator<<(std::ostream& os, basic_ident<Tone, Mod> const&)
{
    using ident_type = basic_ident<Tone, Mod>;
    return os << ident_type::name;
}

template<class Scale, std::enable_if_t<is_scale_v<Scale>, int> = 0>
inline std::ostream& operator<<(std::ostream& os, Scale const& v)
{
    return io_detail::operator<<(os, v);
}

template<class Ident, class KeyFeel>
inline std::ostream& operator<<(std::ostream& os, basic_key<key_ident<Ident, KeyFeel>> const&)
{
    using key_type = basic_key<key_ident<Ident, KeyFeel>>;

    os
        << "[Key]\n"
        << "name: " << key_type::name << "\n"
        << "key sign: " << key_type::key_sign_type::symbol << "\n"
        << "diatonic chords: "
    ;

    io_detail::operator<<(os, typename key_type::diatonic_chords_seq{}) << "\n";

    os
        << "[Key scale]\n" << typename key_type::key_scale_type{} << "\n[/Key scale]\n"
        << "[/Key]"
        //<< ompu::music::make_resolve_in_key(key_type::key_scale_type{}, key_type{})
    ;

    return os;
}

}} // ompu
