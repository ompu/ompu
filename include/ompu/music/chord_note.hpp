#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/mod.hpp"
#include "ompu/music/tone_types.hpp"


namespace ompu { namespace music {

namespace detail {

template<unsigned ID>
struct chord_note_id_id_to_relative_height;

template<> struct chord_note_id_id_to_relative_height<1 > { using type = relative_height<0 >; };
template<> struct chord_note_id_id_to_relative_height<2 > { using type = relative_height<2 >; };
template<> struct chord_note_id_id_to_relative_height<3 > { using type = relative_height<4 >; };
template<> struct chord_note_id_id_to_relative_height<4 > { using type = relative_height<5 >; };
template<> struct chord_note_id_id_to_relative_height<5 > { using type = relative_height<7 >; };
template<> struct chord_note_id_id_to_relative_height<6 > { using type = relative_height<9 >; };
template<> struct chord_note_id_id_to_relative_height<7 > { using type = relative_height<11>; };
template<> struct chord_note_id_id_to_relative_height<9 > { using type = relative_height<14>; };
template<> struct chord_note_id_id_to_relative_height<11> { using type = relative_height<17>; };
template<> struct chord_note_id_id_to_relative_height<13> { using type = relative_height<21>; };

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

    using height_type = relative_height<
        detail::chord_note_id_id_to_relative_height<ID>::type::unsafe_offset +
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
