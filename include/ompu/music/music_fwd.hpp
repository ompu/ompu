#pragma once

#include <boost/config.hpp>

#pragma warning(push)
#if BOOST_MSVC
#pragma warning(disable: 4100)
#endif

#include "sprout/string.hpp"
#pragma warning(pop)


#include <type_traits>
#include <utility>


namespace ompu { namespace music {

using ident_height_type = unsigned;
using all_heights_seq = std::make_integer_sequence<ident_height_type, 12>;

namespace mods {

struct none;
struct sharp;
struct flat;

struct dbl_sharp;
struct dbl_flat;

struct natural;

} // mods


namespace idents {

struct C;
struct Cs;
struct Cb;
struct D;
struct Ds;
struct Db;
struct E;
struct Es;
struct Eb;
struct F;
struct Fs;
struct Fb;
struct G;
struct Gs;
struct Gb;
struct A;
struct As;
struct Ab;
struct B;
struct Bs;
struct Bb;

} // idents

namespace key_feels {

struct major;
struct minor;

} // key_feels


namespace scales {

struct wild;
struct canonical;

struct ionian;
struct dorian;
struct phrigian;
struct lydian;
struct mixolydian;
struct aeolian;
struct locrian;

template<class KeyFeel> struct natural;
template<class KeyFeel> struct harmonic;
template<class KeyFeel> struct melodic;

} // scales


template<ident_height_type Height, class Mod>
struct basic_ident;

template<ident_height_type Height, class Mod>
struct ident_traits;

template<class Ident>
struct basic_tone;

template<class ScaledAs, class... Tones>
struct basic_scale;

template<class... Tones>
struct wild_scale;

template<class ScaledAs, class... Scales>
struct dynamic_scale;

template<class Ident, class KeyScale>
struct key_sign;

template<class Ident, class KeyFeel>
struct key_traits;

template<class Ident, class KeyFeel>
struct basic_key;


namespace keys {

template<class KeyFeel> using C = basic_key<idents::C, KeyFeel>;
using CMaj = C<key_feels::major>; using Cmin = C<key_feels::minor>;

template<class KeyFeel> using Cs = basic_key<idents::Cs, KeyFeel>;
using CsMaj = Cs<key_feels::major>; using Csmin = Cs<key_feels::minor>;

template<class KeyFeel> using Cb = basic_key<idents::Cb, KeyFeel>;
using CbMaj = Cb<key_feels::major>; /* using Cbmin = Cb<key_feels::minor>; */

template<class KeyFeel> using D = basic_key<idents::D, KeyFeel>;
using DMaj = D<key_feels::major>; using Dmin = D<key_feels::minor>;

template<class KeyFeel> using Ds = basic_key<idents::Ds, KeyFeel>;
/* using DsMaj = D<key_feels::major>; */ using Dsmin = Ds<key_feels::minor>;

template<class KeyFeel> using Db = basic_key<idents::Db, KeyFeel>;
using DbMaj = Db<key_feels::major>; /* using Dbmin = Db<key_feels::minor>; */

template<class KeyFeel> using E = basic_key<idents::E, KeyFeel>;
using EMaj = E<key_feels::major>; using Emin = E<key_feels::minor>;

//template<class KeyFeel> using Es = basic_key<idents::Es, KeyFeel>;
//using EsMaj = E<key_feels::major>; using Esmin = Es<key_feels::minor>;

template<class KeyFeel> using Eb = basic_key<idents::Eb, KeyFeel>;
using EbMaj = Eb<key_feels::major>; using Ebmin = Eb<key_feels::minor>;

template<class KeyFeel> using F = basic_key<idents::F, KeyFeel>;
using FMaj = F<key_feels::major>; using Fmin = F<key_feels::minor>;

template<class KeyFeel> using Fs = basic_key<idents::Fs, KeyFeel>;
using FsMaj = Fs<key_feels::major>; using Fsmin = Fs<key_feels::minor>;

//template<class KeyFeel> using Fb = basic_key<idents::Fb, KeyFeel>;
//using FbMaj = Fb<key_feels::major>; using Fbmin = Fb<key_feels::minor>;

template<class KeyFeel> using G = basic_key<idents::G, KeyFeel>;
using GMaj = G<key_feels::major>; using Gmin = G<key_feels::minor>;

template<class KeyFeel> using Gs = basic_key<idents::Gs, KeyFeel>;
/* using GsMaj = G<key_feels::major>; */ using Gsmin = Gs<key_feels::minor>;

template<class KeyFeel> using Gb = basic_key<idents::Gb, KeyFeel>;
using GbMaj = Gb<key_feels::major>; /* using Gbmin = Gb<key_feels::minor>; */

template<class KeyFeel> using A = basic_key<idents::A, KeyFeel>;
using AMaj = A<key_feels::major>; using Amin = A<key_feels::minor>;

template<class KeyFeel> using As = basic_key<idents::As, KeyFeel>;
/* using AsMaj = A<key_feels::major>; */ using Asmin = As<key_feels::minor>;

template<class KeyFeel> using Ab = basic_key<idents::Ab, KeyFeel>;
using AbMaj = Ab<key_feels::major>; using Abmin = Ab<key_feels::minor>;

template<class KeyFeel> using B = basic_key<idents::B, KeyFeel>;
using BMaj = B<key_feels::major>; using Bmin = B<key_feels::minor>;

//template<class KeyFeel> using Bs = basic_key<idents::Bs, KeyFeel>;
//using BsMaj = Bs<key_feels::major>; using Bsmin = Bs<key_feels::minor>;

template<class KeyFeel> using Bb = basic_key<idents::Bb, KeyFeel>;
using BbMaj = Bb<key_feels::major>; using Bbmin = Bb<key_feels::minor>;

} // keys


namespace contexts {

struct auto_select;

struct key;
struct scale;
struct chord;

} // contexts

}} // ompu