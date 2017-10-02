#pragma once

#include "ompu/music/scale.hpp"
#include "ompu/music/key_traits.hpp"

#include "saya/zed/seq.hpp"


namespace ompu { namespace music {


template<class Ident, class KeyFeel, class Traits = key_traits<Ident, KeyFeel>>
struct basic_key
{
    using traits_type = Traits;
    using traits_type::ident_type;
    using traits_type::key_feel;
    using traits_type::key_scale_type;
    using traits_type::key_sign_type;
};


namespace keys {

template<class KeyType> using C = basic_key<idents::C, KeyType>;
using CMaj = C<key_feels::Major>; using Cmin = C<key_feels::Minor>;

template<class KeyType> using Cs = basic_key<idents::Cs, KeyType>;
using CsMaj = Cs<key_feels::Major>; using Csmin = Cs<key_feels::Minor>;

template<class KeyType> using Cb = basic_key<idents::Cb, KeyType>;
using CbMaj = Cb<key_feels::Major>; /* using Cbmin = Cb<key_feels::Minor>; */

template<class KeyType> using D = basic_key<idents::D, KeyType>;
using DMaj = D<key_feels::Major>; using Dmin = D<key_feels::Minor>;

template<class KeyType> using Ds = basic_key<idents::Ds, KeyType>;
/* using DsMaj = D<key_feels::Major>; */ using Dsmin = Ds<key_feels::Minor>;

template<class KeyType> using Db = basic_key<idents::Db, KeyType>;
using DbMaj = Db<key_feels::Major>; /* using Dbmin = Db<key_feels::Minor>; */

template<class KeyType> using E = basic_key<idents::E, KeyType>;
using EMaj = E<key_feels::Major>; using Emin = E<key_feels::Minor>;

//template<class KeyType> using Es = basic_key<idents::Es, KeyType>;
//using EsMaj = E<key_feels::Major>; using Esmin = Es<key_feels::Minor>;

template<class KeyType> using Eb = basic_key<idents::Eb, KeyType>;
using EbMaj = Eb<key_feels::Major>; using Ebmin = Eb<key_feels::Minor>;

template<class KeyType> using F = basic_key<idents::F, KeyType>;
using FMaj = F<key_feels::Major>; using Fmin = F<key_feels::Minor>;

template<class KeyType> using Fs = basic_key<idents::Fs, KeyType>;
using FsMaj = Fs<key_feels::Major>; using Fsmin = Fs<key_feels::Minor>;

//template<class KeyType> using Fb = basic_key<idents::Fb, KeyType>;
//using FbMaj = Fb<key_feels::Major>; using Fbmin = Fb<key_feels::Minor>;

template<class KeyType> using G = basic_key<idents::G, KeyType>;
using GMaj = G<key_feels::Major>; using Gmin = G<key_feels::Minor>;

template<class KeyType> using Gs = basic_key<idents::Gs, KeyType>;
/* using GsMaj = G<key_feels::Major>; */ using Gsmin = Gs<key_feels::Minor>;

template<class KeyType> using Gb = basic_key<idents::Gb, KeyType>;
using GbMaj = Gb<key_feels::Major>; /* using Gbmin = Gb<key_feels::Minor>; */

template<class KeyType> using A = basic_key<idents::A, KeyType>;
using AMaj = A<key_feels::Major>; using Amin = A<key_feels::Minor>;

template<class KeyType> using As = basic_key<idents::As, KeyType>;
/* using AsMaj = A<key_feels::Major>; */ using Asmin = As<key_feels::Minor>;

template<class KeyType> using Ab = basic_key<idents::Ab, KeyType>;
using AbMaj = Ab<key_feels::Major>; using Abmin = Ab<key_feels::Minor>;

template<class KeyType> using B = basic_key<idents::B, KeyType>;
using BMaj = B<key_feels::Major>; using Bmin = B<key_feels::Minor>;

//template<class KeyType> using Bs = basic_key<idents::Bs, KeyType>;
//using BsMaj = Bs<key_feels::Major>; using Bsmin = Bs<key_feels::Minor>;

template<class KeyType> using Bb = basic_key<idents::Bb, KeyType>;
using BbMaj = Bb<key_feels::Major>; using Bbmin = Bb<key_feels::Minor>;

} // keys


namespace detail {

template<class KeyFeel>
struct basic_canonical_key_mods_impl;

template<>
struct basic_canonical_key_mods_impl<key_feels::Major>
{
    using type = std::tuple<
        mods::None, // C
        mods::Flat, // Db
        mods::None, // D
        mods::Flat, // Eb
        mods::None, // E
        mods::None, // F
        mods::Sharp, // F#
        mods::None, // G
        mods::Flat, // Ab
        mods::None, // A
        mods::Flat, // Bb
        mods::None // B
    >;
};

template<>
struct basic_canonical_key_mods_impl<key_feels::Minor>
{
    using type = std::tuple<
        mods::None, // Cm
        mods::Sharp, // C#m
        mods::None, // Dm
        mods::Flat, // Eb
        mods::None, // Em
        mods::None, // Fm
        mods::Sharp, // F#
        mods::None, // Gm
        mods::Sharp, // G#m
        mods::None, // Am
        mods::Flat, // Bb
        mods::None // Bm
    >;
};

template<class KeyFeel>
using basic_canonical_key_mods_impl_t = typename basic_canonical_key_mods_impl<KeyFeel>::type;

} // detail


namespace detail {

template<class Key>
struct force_enharmonic_key;

template<class Ident, class KeyFeel>
struct force_enharmonic_key<basic_key<Ident, KeyFeel>>
{
    using type = basic_key<canonical_ident_t<Ident>, KeyFeel>;
};

template<class Key>
using force_enharmonic_key_t = typename force_enharmonic_key<Key>::type;


template<class Key>
struct has_enharmonic_key_impl : std::false_type {};

template<> struct has_enharmonic_key_impl<keys::BMaj> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::FsMaj> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::CsMaj> : std::true_type {};

template<> struct has_enharmonic_key_impl<keys::Gsmin> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::Dsmin> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::Asmin> : std::true_type {};

} // detail

template<class Key>
struct has_enharmonic_key
    : std::conditional_t<
        detail::has_enharmonic_key_impl<Key>::value ||
        detail::has_enharmonic_key_impl<detail::force_enharmonic_key_t<Key>>::value,
        std::true_type,
        std::false_type
    >
{};

template<class Key>
constexpr bool has_enharmonic_key_v = has_enharmonic_key<Key>::value;


template<class Key, class Enabled = void>
struct enharmonic_key { using type = void; };

template<class Ident, class KeyFeel>
struct enharmonic_key<
    basic_key<Ident, KeyFeel>,
    std::enable_if_t<
        has_enharmonic_key_v<basic_key<Ident, KeyFeel>>
    >
>
{
    using type = basic_key<
        tone_ident_t<
            Ident::root,
            std::conditional_t<
                Ident::sharped, mods::Flat, mods::Sharp
            >
        >,
        KeyFeel
    >;
};

template<class Key>
using enharmonic_key_t = typename enharmonic_key<Key>::type;


template<class Key, class Enabled = void>
struct enharmonic_key_pair;

template<class Key>
struct enharmonic_key_pair<Key, std::enable_if_t<has_enharmonic_key_v<Key>>>
{
    using type = std::tuple<Key, enharmonic_key_t<Key>>;
};

template<class Key>
struct enharmonic_key_pair<Key, std::enable_if_t<!has_enharmonic_key_v<Key>>>
{
    using type = std::tuple<Key>;
};

template<class Key>
using enharmonic_key_pair_t = typename enharmonic_key_pair<Key>::type;


namespace detail {

template<class...>
struct any_key_impl;

template<class KeyFeel, unsigned... Roots, class... Mods>
struct any_key_impl<KeyFeel, std::integer_sequence<unsigned, Roots...>, std::tuple<Mods...>>
{
    using type = boost::variant<
        saya::zed::t_seq_concat_t<
            enharmonic_key_pair_t<basic_key<basic_tone_ident<Roots, Mods>, KeyFeel>>...
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
            detail::basic_canonical_key_mods_impl_t<KeyFeels>
        >...
    >;
};

template<class... KeyFeels>
using any_keys_seq_t = typename any_keys_seq<KeyFeels...>::type;

template<class... KeyFeels>
using any_keys_variant_t = saya::zed::t_seq_variant_t<any_keys_seq_t<KeyFeels...>>;

}} // ompu
