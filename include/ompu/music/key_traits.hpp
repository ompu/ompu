#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/key_feel.hpp"
#include "ompu/music/scale.hpp"


namespace ompu { namespace music {

template<class Ident, class KeyScale>
struct key_sign
{
private:
    using sharped_key_scale_in_C_type = cvt::detail::force_modded_scale_in_C_t<KeyScale, mods::sharp>;
    using flatted_key_scale_in_C_type = cvt::detail::force_modded_scale_in_C_t<KeyScale, mods::flat>;
    using canonical_key_mod_type = cvt::detail::canonical_key_mod_t<
        typename Ident::mod_type,
        sharped_key_scale_in_C_type,
        flatted_key_scale_in_C_type
    >;

public:
    using mod_type = typename canonical_key_mod_type::mod_type;
    static constexpr auto assoc_sharps = canonical_key_mod_type::assoc_sharps;
    static constexpr auto assoc_flats = canonical_key_mod_type::assoc_flats;
    static constexpr auto assoc_lesser_mods = canonical_key_mod_type::assoc_lesser_mods;
};

template<class Ident, class KeyFeel>
struct key_traits
{
    using ident_type = Ident;

    using key_feel = KeyFeel;
    using key_scale_type = scales::make_natural<Ident, KeyFeel>;

    using key_sign_type = key_sign<Ident, key_scale_type>;

    static constexpr bool is_major_key = is_major_feel_v<KeyFeel>;
    static constexpr bool is_minor_key = is_minor_feel_v<KeyFeel>;
};

}} // ompu
