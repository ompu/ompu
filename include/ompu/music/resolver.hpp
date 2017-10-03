#pragma once

#include "ompu/music.hpp"

#include "saya/zed/fold.hpp"


namespace ompu { namespace music {

namespace contexts {

struct auto_select {}; // really?

struct key {};
struct scale {};
struct chord {};

} // contexts


namespace detail {

template<class Scale, class...>
struct scale_role_fetcher;

template<class Scale, class... CompareToRoles>
struct scale_role_fetcher<Scale, std::tuple<CompareToRoles...>>
{
    using scaled_as_type = typename Scale::scaled_as_type;
    using type = /* saya::zed::compact_t< */
        saya::zed::maybe_empty_seq<
            std::conditional_t<
                std::is_same_v<scaled_as_type, CompareToRoles>,
                CompareToRoles,
                CompareToRoles // saya::zed::void_elem
            >...
        /*>*/
    >;
};


template<class Identity, class KeyLikeContext, class Enabled = void>
struct key_context_resolver;

template<class ScaleLikeIdentity, class KeyLikeContext>
struct key_context_resolver<
    ScaleLikeIdentity,
    KeyLikeContext
>
{
    using scale_type = ScaleLikeIdentity;
    using key_type = KeyLikeContext;

    using type = typename scale_role_fetcher<
        scale_type,
        typename key_type::known_scale_roles
    >::type;
};

template<class Identity, class Context>
using key_context_resolver_t = typename key_context_resolver<Identity, Context>::type;


template<class ContextAs, class Identity, class Context, class Enabled = void>
struct context_resolver;

template<class Identity, class Context>
struct context_resolver<
    contexts::key, Identity, Context
>
{
    using type = key_context_resolver_t<Identity, Context>;
};


template<class Identity, class Ident, class KeyFeel>
struct context_resolver<
    contexts::auto_select, Identity, basic_key<Ident, KeyFeel>
>
{
    using type = key_context_resolver_t<Identity, basic_key<Ident, KeyFeel>>;
};

template<class ContextAs, class Identity, class Context>
using context_resolver_t = typename context_resolver<ContextAs, Identity, Context>::type;

} // detail


template<class ContextAs, class Identity, class Context>
inline auto make_resolve_in_context(ContextAs, Identity const&, Context const&)
{
    return detail::context_resolver_t<ContextAs, Identity, Context>{};
}

template<class Identity, class Context>
inline auto make_resolve_in_key(Identity const&, Context const&)
{
    return detail::context_resolver_t<contexts::key, Identity, Context>{};
}

}} // ompu
