#pragma once

#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace key_feels {

struct major
{
    static constexpr auto name = sprout::to_string("Major");
};

struct minor
{
    static constexpr auto name = sprout::to_string("Minor");
};

} // key_feels


namespace detail {

template<class KeyFeel>
struct friendly_key_mods;

template<>
struct friendly_key_mods<key_feels::major>
{
    using type = std::tuple<
        mods::none, // C
        mods::flat, // Db
        mods::none, // D
        mods::flat, // Eb
        mods::none, // E
        mods::none, // F
        mods::sharp, // F#
        mods::none, // G
        mods::flat, // Ab
        mods::none, // A
        mods::flat, // Bb
        mods::none // B
    >;
};

template<>
struct friendly_key_mods<key_feels::minor>
{
    using type = std::tuple<
        mods::none, // Cm
        mods::sharp, // C#m
        mods::none, // Dm
        mods::flat, // Eb
        mods::none, // Em
        mods::none, // Fm
        mods::sharp, // F#
        mods::none, // Gm
        mods::sharp, // G#m
        mods::none, // Am
        mods::flat, // Bb
        mods::none // Bm
    >;
};

template<class KeyFeel>
using friendly_key_mods_t = typename friendly_key_mods<KeyFeel>::type;

} // detail

}} // ompu
