#pragma once

#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace detail {

// FIXME
#if 0

template<ident_height_type Height, class Mod, class KeyFeel>
struct key_sign_mod_impl;

template<ident_height_type Height, class KeyFeel>
struct key_sign_mod_impl<0, mods::none, key_feels::major>
{
    using type = mods::none; // C
};

template<ident_height_type Height, class Mod>
struct key_sign_mod_impl<Height, Mod, key_feels::minor>
{
    using type = key_sign_mod_impl<cvt::detail::height_shift_v<Height, -9>, mods::
};
#endif

} // detail


template<class Ident, class KeyFeel>
struct basic_key_sign
{

};

}} // ompu
