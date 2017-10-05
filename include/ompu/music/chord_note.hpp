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


template<bool IsTension, unsigned ID, int ModOffset>
struct chord_note_id
{
    static constexpr auto is_tension = IsTension;
    static constexpr auto unsafe_id = ID;
    static constexpr auto unsafe_mod_offset = ModOffset;

    using height_type = relative_height<
        detail::chord_note_id_id_to_relative_height<ID>::type::unsafe_offset +
        ModOffset
    >;

    static constexpr auto const& id_str = detail::number_names<ID>::name;
};


template<class ID>
struct basic_chord_note
{
    using id_type = ID;
    static constexpr bool is_tension = ID::is_tension;
};

template<unsigned TensionID, class Mod>
struct basic_chord_tension
    : basic_chord_note<
        chord_note_id<true, TensionID, Mod::offset>
    >
{
    static constexpr auto tension_id = TensionID;
    using mod_type = Mod;
};


namespace chord_notes {

struct M3
    : basic_chord_note<chord_note_id<false, 3>>
{};

struct m3
    : basic_chord_note<chord_note_id<false, 3, -1>>
{};

struct P5
    : basic_chord_note<chord_note_id<false, 5>>
{};

struct b5
    : basic_chord_note<chord_note_id<false, 5, -1>>
{};

struct aug5
    : basic_chord_note<chord_note_id<false, 5, +1>>
{};

struct M6
    : basic_chord_note<chord_note_id<false, 6>>
{};

struct m6
    : basic_chord_note<chord_note_id<false, 6, -1>>
{};

struct M7
    : basic_chord_note<chord_note_id<false, 7>>
{};

struct m7
    : basic_chord_note<chord_note_id<false, 7, -1>>
{};

struct dim7
    : basic_chord_note<chord_note_id<false, 7, -2>>
{};


// tensions ------------------------------------


struct P9
    : basic_chord_tension<9, mods::none>
{};

struct s9
    : basic_chord_tension<9, mods::sharp>
{};

// struct b10 : basic_chord_tension<10, mods::flat> {};

struct s11
    : basic_chord_tension<11, mods::sharp>
{};

struct P13
    : basic_chord_tension<13, mods::none>
{};

struct s13
    : basic_chord_tension<13, mods::sharp>
{};

struct b13
    : basic_chord_tension<13, mods::flat>
{};

} // chord_notes

}} // ompu
