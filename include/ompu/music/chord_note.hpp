#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/mod.hpp"
#include "ompu/music/tone_types.hpp"


namespace ompu { namespace music {

namespace detail {

template<unsigned ID>
struct chord_note_id_id_to_tone_offset;

template<> struct chord_note_id_id_to_tone_offset<1 > { using type = tone_offset<0 >; };
template<> struct chord_note_id_id_to_tone_offset<2 > { using type = tone_offset<2 >; };
template<> struct chord_note_id_id_to_tone_offset<3 > { using type = tone_offset<4 >; };
template<> struct chord_note_id_id_to_tone_offset<4 > { using type = tone_offset<5 >; };
template<> struct chord_note_id_id_to_tone_offset<5 > { using type = tone_offset<7 >; };
template<> struct chord_note_id_id_to_tone_offset<6 > { using type = tone_offset<9 >; };
template<> struct chord_note_id_id_to_tone_offset<7 > { using type = tone_offset<11>; };
template<> struct chord_note_id_id_to_tone_offset<9 > { using type = tone_offset<14>; };
template<> struct chord_note_id_id_to_tone_offset<11> { using type = tone_offset<17>; };
template<> struct chord_note_id_id_to_tone_offset<13> { using type = tone_offset<21>; };

} // detail


template<class Note> struct chord_note_name;

template<> struct chord_note_name<chord_note_id<false, 3>>
{ static constexpr auto const& name = detail::symbol_names::empty; };
template<> struct chord_note_name<chord_note_id<false, 3, mods::flat>>
{ static constexpr auto name = sprout::to_string("m"); };
template<> struct chord_note_name<chord_note_id<false, 5>>
{ static constexpr auto const& name = detail::symbol_names::empty; };
template<> struct chord_note_name<chord_note_id<false, 5, mods::flat>>
{ static constexpr auto name = sprout::to_string("-5"); };
template<> struct chord_note_name<chord_note_id<false, 5, mods::sharp>>
{ static constexpr auto const& name = detail::symbol_names::aug; };
template<> struct chord_note_name<chord_note_id<false, 6>>
{ static constexpr auto name = sprout::to_string("6"); };
template<> struct chord_note_name<chord_note_id<false, 7>>
{ static constexpr auto name = sprout::to_string("M") + detail::number_names<7>::name; };
template<> struct chord_note_name<chord_note_id<false, 7, mods::flat>>
{ static constexpr auto const& name = detail::number_names<7>::name; };
template<> struct chord_note_name<chord_note_id<false, 7, mods::dbl_flat>>
{ static constexpr auto name = detail::symbol_names::dim + detail::number_names<7>::name; };

template<> struct chord_note_name<chord_note_id<true, 9, mods::none>>
{ static constexpr auto name = sprout::to_string("add") + detail::number_names<9>::name; };
template<> struct chord_note_name<chord_note_id<true, 9, mods::sharp>>
{ static constexpr auto name = detail::symbol_names::sharp + detail::number_names<9>::name;};
template<> struct chord_note_name<chord_note_id<true, 11, mods::sharp>>
{ static constexpr auto name = detail::symbol_names::sharp + detail::number_names<11>::name; };
template<> struct chord_note_name<chord_note_id<true, 13, mods::none>>
{ static constexpr auto const& name = detail::number_names<13>::name; };
template<> struct chord_note_name<chord_note_id<true, 13, mods::sharp>>
{ static constexpr auto name = detail::symbol_names::sharp + detail::number_names<13>::name; };
template<> struct chord_note_name<chord_note_id<true, 13, mods::flat>>
{ static constexpr auto name = detail::symbol_names::flat + detail::number_names<13>::name; };

template<class ID>
struct chord_note_name<
    basic_chord_note<ID>
>
{
    static constexpr auto const& name = chord_note_name<ID>::name;
};


template<bool IsTension, unsigned ID, class Mod>
struct chord_note_id
{
    static constexpr auto is_tension = IsTension;
    static constexpr auto unsafe_id = ID;
    using mod_type = Mod;

    using height_type = tone_offset<
        detail::chord_note_id_id_to_tone_offset<ID>::type::value +
        Mod::offset
    >;
};

template<class ID>
struct basic_chord_note
{
    using id_type = ID;
    using height_type = typename ID::height_type;
    using mod_type = typename ID::mod_type;
    static constexpr bool is_tension = ID::is_tension;

    // static constexpr auto const& name = chord_note_name<ID>::name;
};

}} // ompu
