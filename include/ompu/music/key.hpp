#pragma once

#include "ompu/music/scale.hpp"


namespace ompu { namespace music {

namespace keys {

struct Major {};
struct Minor {};

} // keys


template<class RootIdent, class KeyFeel>
struct basic_key_traits;

template<class RootIdent>
struct basic_key_traits<RootIdent, keys::Major>
{
    using root_ident_type = RootIdent;
    using key_feel = keys::Major;
    using key_scale_type = scales::ionian<RootIdent>;

    static constexpr bool is_major_key = true;
    static constexpr bool is_minor_key = false;
};

template<class RootIdent>
struct basic_key_traits<RootIdent, keys::Minor>
{
    using root_ident_type = RootIdent;
    using key_feel = keys::Minor;
    using key_scale_type = scales::aeolian<RootIdent>;

    static constexpr bool is_major_key = false;
    static constexpr bool is_minor_key = true;
};

template<class RootIdent, class KeyFeel, class Traits = basic_key_traits<RootIdent, KeyFeel>>
struct basic_key
{
    using traits_type = Traits;
};

namespace keys {

template<class KeyType> using C = basic_key<idents::C, KeyType>;
using CMaj = C<keys::Major>; using Cmin = C<keys::Minor>;

template<class KeyType> using Cs = basic_key<idents::Cs, KeyType>;
using CsMaj = Cs<keys::Major>; using Csmin = Cs<keys::Minor>;

template<class KeyType> using Cb = basic_key<idents::Cb, KeyType>;
using CbMaj = Cb<keys::Major>; /* using Cbmin = Cb<keys::Minor>; */

template<class KeyType> using D = basic_key<idents::D, KeyType>;
using DMaj = D<keys::Major>; using Dmin = D<keys::Minor>;

template<class KeyType> using Ds = basic_key<idents::Ds, KeyType>;
/* using DsMaj = D<keys::Major>; */ using Dsmin = Ds<keys::Minor>;

template<class KeyType> using Db = basic_key<idents::Db, KeyType>;
using DbMaj = Db<keys::Major>; /* using Dbmin = Db<keys::Minor>; */

template<class KeyType> using E = basic_key<idents::E, KeyType>;
using EMaj = E<keys::Major>; using Emin = E<keys::Minor>;

//template<class KeyType> using Es = basic_key<idents::Es, KeyType>;
//using EsMaj = E<keys::Major>; using Esmin = Es<keys::Minor>;

template<class KeyType> using Eb = basic_key<idents::Eb, KeyType>;
using EbMaj = Eb<keys::Major>; using Ebmin = Eb<keys::Minor>;

template<class KeyType> using F = basic_key<idents::F, KeyType>;
using FMaj = F<keys::Major>; using Fmin = F<keys::Minor>;

template<class KeyType> using Fs = basic_key<idents::Fs, KeyType>;
using FsMaj = Fs<keys::Major>; using Fsmin = Fs<keys::Minor>;

//template<class KeyType> using Fb = basic_key<idents::Fb, KeyType>;
//using FbMaj = Fb<keys::Major>; using Fbmin = Fb<keys::Minor>;

template<class KeyType> using G = basic_key<idents::G, KeyType>;
using GMaj = G<keys::Major>; using Gmin = G<keys::Minor>;

template<class KeyType> using Gs = basic_key<idents::Gs, KeyType>;
/* using GsMaj = G<keys::Major>; */ using Gsmin = Gs<keys::Minor>;

template<class KeyType> using Gb = basic_key<idents::Gb, KeyType>;
using GbMaj = Gb<keys::Major>; /* using Gbmin = Gb<keys::Minor>; */

template<class KeyType> using A = basic_key<idents::A, KeyType>;
using AMaj = A<keys::Major>; using Amin = A<keys::Minor>;

template<class KeyType> using As = basic_key<idents::As, KeyType>;
/* using AsMaj = A<keys::Major>; */ using Asmin = As<keys::Minor>;

template<class KeyType> using Ab = basic_key<idents::Ab, KeyType>;
using AbMaj = Ab<keys::Major>; using Abmin = Ab<keys::Minor>;

template<class KeyType> using B = basic_key<idents::B, KeyType>;
using BMaj = B<keys::Major>; using Bmin = B<keys::Minor>;

//template<class KeyType> using Bs = basic_key<idents::Bs, KeyType>;
//using BsMaj = Bs<keys::Major>; using Bsmin = Bs<keys::Minor>;

template<class KeyType> using Bb = basic_key<idents::Bb, KeyType>;
using BbMaj = Bb<keys::Major>; using Bbmin = Bb<keys::Minor>;

} // keys

}} // ompu
