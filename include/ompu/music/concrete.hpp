#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/key.hpp"
#include "ompu/music/type_traits.hpp"

#include <boost/variant/variant_fwd.hpp>


namespace ompu { namespace music { namespace concretes {

namespace detail {

template<class...>
struct any_key_impl;

template<class KeyFeel, tone_height_type... Roots, class... Mods>
struct any_key_impl<KeyFeel, std::integer_sequence<tone_height_type, Roots...>, std::tuple<Mods...>>
{
    using type = boost::variant<
        saya::zed::t_seq_concat_t<
            enharmonic_key_pair_t<basic_key<basic_ident<Roots, Mods>, KeyFeel>>...
        >
    >;
};

} // detail

template<class... KeyFeels>
struct any_keys_seq
{
    using type = saya::zed::t_seq_concat_t<
        detail::any_key_impl<
            KeyFeels,
            all_heights_seq,
            detail::friendly_key_mods_t<KeyFeels>
        >...
    >;
};

template<class... KeyFeels>
using any_keys_seq_t = typename any_keys_seq<KeyFeels...>::type;

template<class... KeyFeels>
using any_keys_variant_t = saya::zed::t_seq_variant_t<any_keys_seq_t<KeyFeels...>>;

}}} // ompu
