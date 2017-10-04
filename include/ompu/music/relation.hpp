#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/degree.hpp"


namespace ompu { namespace music {

namespace detail {

} // detail

template<class OriginalKeyFeel, class Degree, class FeelMod>
struct relation
{
    using degree_type = Degree;
    using original_key_feel = OriginalKeyFeel;
    using feel_mod = FeelMod;
    using related_key_feel = cvt::modded_feel_t<OriginalKeyFeel, FeelMod>;
};


namespace relations {

template<class KeyFeel>
struct tonic
    : relation<KeyFeel, degrees::I, feel_mods::same>
{
    static constexpr auto const& name = detail::symbol_names::tonic_r;
};

template<class KeyFeel>
struct parallel
    : relation<KeyFeel, degrees::I, feel_mods::opposite>
{
    static constexpr auto const& name = detail::symbol_names::parallel_r;
};

template<>
struct relative<key_feels::major>
    : relation<key_feels::major, degrees::VI, feel_mods::opposite>
{
    static constexpr auto const& name = detail::symbol_names::relative_r;
};
template<>
struct relative<key_feels::minor>
    : relation<key_feels::minor, degrees::III, feel_mods::opposite>
{
    static constexpr auto const& name = detail::symbol_names::relative_r;
};


template<class KeyFeel>
struct dominant
    : relation<KeyFeel, degrees::V, feel_mods::same>
{
    static constexpr auto const& name = detail::symbol_names::dominant_r;
};

template<class KeyFeel>
struct sub_dominant
    : relation<KeyFeel, degrees::IV, feel_mods::same>
{
    static constexpr auto const& name = detail::symbol_names::sub_dominant_r;
};

} // relations

}} // ompu
