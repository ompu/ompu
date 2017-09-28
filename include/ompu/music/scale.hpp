#pragma once

#include "ompu/music/tone.hpp"

#include "saya/zed/fold.hpp"


namespace ompu { namespace music {

namespace detail {

template<class... Idents>
struct basic_scale_def {};

template<unsigned... Heights, class... Mods>
using make_basic_scale_def = basic_scale_def<make_basic_tone_ident<Heights, Mods>...>;


using ionian_scale_def = make_basic_scale_def<
    0, 2, 4, 5, 7, 9, 11,
    mods::None, mods::None, mods::None, mods::None, mods::None, mods::None, mods::None
>;
using dorian_scale_def = make_basic_scale_def<
    0, 2, 3, 5, 7, 9, 10,
    mods::None, mods::None, mods::Flat, mods::None, mods::None, mods::None, mods::Flat
>;
using phrigian_scale_def = make_basic_scale_def<
    0, 1, 3, 5, 7, 8, 10,
    mods::None, mods::Flat, mods::Flat, mods::None, mods::None, mods::Flat, mods::Flat
>;
using lydian_scale_def = make_basic_scale_def<
    0, 2, 4, 6, 7, 9, 11,
    mods::None, mods::None, mods::None, mods::Sharp, mods::None, mods::None, mods::None
>;
using mixolydian_scale_def = make_basic_scale_def<
    0, 2, 4, 5, 7, 9, 10,
    mods::None, mods::None, mods::None, mods::None, mods::None, mods::None, mods::Flat
>;
using aeolian_scale_def = make_basic_scale_def<
    0, 2, 3, 5, 7, 8, 10,
    mods::None, mods::None, mods::Flat, mods::None, mods::None, mods::Flat, mods::Flat
>;
using locrian_scale_def = make_basic_scale_def<
    0, 1, 3, 5, 6, 8, 10,
    mods::None, mods::Flat, mods::Flat, mods::None, mods::Flat, mods::Flat, mods::Flat
>;

} // detail

template<class...>
struct basic_scale;

template<class... Idents>
struct basic_scale<basic_tone<Idents>...>
{
    static constexpr unsigned tones_count = sizeof...(Idents);
    static constexpr auto assoc_sharps = saya::zed::fold_add_v<unsigned, detail::sharp_counter<Idents>::value...>;
    static constexpr auto assoc_flats = saya::zed::fold_add_v<unsigned, detail::flat_counter<Idents>::value...>;
};

namespace detail {

template<class...>
struct make_scale_impl;

template<class RootIdent, unsigned... Heights, class... Mods>
struct make_scale_impl<RootIdent, basic_scale_def<basic_tone_ident<Heights, Mods>...>>
{
    using root_type = RootIdent;
    using type = basic_scale<
        basic_tone<
            basic_tone_ident<(RootIdent::value + Heights) % 12, Mods>
        >...
    >;
};

} // detail


namespace scales {

template<class RootIdent>
using ionian = typename detail::make_scale_impl<RootIdent, detail::ionian_scale_def>::type;

template<class RootIdent>
using dorian = typename detail::make_scale_impl<RootIdent, detail::dorian_scale_def>::type;

template<class RootIdent>
using phrigian = typename detail::make_scale_impl<RootIdent, detail::phrigian_scale_def>::type;

template<class RootIdent>
using lydian = typename detail::make_scale_impl<RootIdent, detail::lydian_scale_def>::type;

template<class RootIdent>
using mixolydian = typename detail::make_scale_impl<RootIdent, detail::mixolydian_scale_def>::type;

template<class RootIdent>
using aeolian = typename detail::make_scale_impl<RootIdent, detail::aeolian_scale_def>::type;

template<class RootIdent>
using locrian = typename detail::make_scale_impl<RootIdent, detail::locrian_scale_def>::type;

} // scales

}} // ompu
