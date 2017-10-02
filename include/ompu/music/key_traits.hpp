#pragma once

#include "ompu/music/key_feels.hpp"
#include "ompu/music/scale.hpp"


namespace ompu { namespace music {

namespace detail {

template<class RootMod, class SharpedScale, class FlattedScale>
struct canonical_key_mod
{
    using mod_type = std::conditional_t<
        SharpedScale::assoc_sharps == 0 && FlattedScale::assoc_flats == 0,
        mods::None,
        std::conditional_t<
            SharpedScale::assoc_sharps == FlattedScale::assoc_flats,
            RootMod,
            std::conditional_t<
                SharpedScale::assoc_sharps < FlattedScale::assoc_flats,
                mods::Sharp,
                mods::Flat
            >
        >
    >;

    static constexpr auto assoc_sharps = SharpedScale::assoc_sharps;
    static constexpr auto assoc_flats = FlattedScale::assoc_flats;

    static constexpr auto assoc_lesser_mods = assoc_sharps <= assoc_flats ? assoc_sharps : assoc_flats;
};

template<class RootMod, class SharpedScale, class FlattedScale>
using canonical_key_mod_t = typename canonical_key_mod<RootMod, SharpedScale, FlattedScale>::type;

} //detail


template<class RootIdent, class KeyScale>
struct key_sign
{
private:
    using sharped_key_scale_in_C_type = detail::modded_scale_in_C_t<KeyScale, mods::Sharp>;
    using flatted_key_scale_in_C_type = detail::modded_scale_in_C_t<KeyScale, mods::Flat>;
    using canonical_key_mod_type = detail::canonical_key_mod_t<
        typename RootIdent::mod_type,
        sharped_key_scale_in_C_type,
        flatted_key_scale_in_C_type
    >;

public:
    using mod_type = typename canonical_key_mod_type::mod_type;
    static constexpr auto assoc_sharps = canonical_key_mod_type::assoc_sharps;
    static constexpr auto assoc_flats = canonical_key_mod_type::assoc_flats;
    static constexpr auto assoc_lesser_mods = canonical_key_mod_type::assoc_lesser_mods;
};

template<class Root, class KeyFeel>
struct key_traits
{
    using root_type = Root;
    using root_ident_type = typename root_type::ident_type;

    using key_feel = KeyFeel;
    using key_scale_type = scales::natural<root_ident_type, KeyFeel>;

    using key_sign_type = key_sign<root_ident_type, key_scale_type>;

    static constexpr bool is_major_key = is_major_feel_v<KeyFeel>;
    static constexpr bool is_minor_key = is_minor_feel_v<KeyFeel>;
};

}} // ompu
