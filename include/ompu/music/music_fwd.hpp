#pragma once

#include <boost/config.hpp>

#pragma warning(push)
#if BOOST_MSVC
#pragma warning(disable: 4100)
#endif

#include "sprout/string.hpp"
#pragma warning(pop)

#include "ompu/music/detail/symbol_names.hpp"

#include "saya/zed/seq.hpp"

#include <tuple>
#include <type_traits>
#include <utility>


namespace ompu { namespace music {

template<unsigned Height>
struct tone_height;

template<int UnsafeOffset>
struct relative_height;

template<int Offset>
struct tone_offset;

template<class ToneHeight>
struct basic_tone;


namespace detail {

template<unsigned Height>
using make_tone = basic_tone<tone_height<Height>>;

} // detail


namespace tones {

using C  = music::detail::make_tone<0>;
using Cs = music::detail::make_tone<1>;
using D  = music::detail::make_tone<2>;
using Ds = music::detail::make_tone<3>;
using E  = music::detail::make_tone<4>;
using F  = music::detail::make_tone<5>;
using Fs = music::detail::make_tone<6>;
using G  = music::detail::make_tone<7>;
using Gs = music::detail::make_tone<8>;
using A  = music::detail::make_tone<9>;
using As = music::detail::make_tone<10>;
using B  = music::detail::make_tone<11>;

} // tones


template<class... Tones>
struct tone_set;

template<class... ToneHeights>
using make_tone_set = tone_set<basic_tone<ToneHeights>...>;

namespace detail {

template<unsigned ... ToneHeights>
using make_tone_set = tone_set<basic_tone<tone_height<ToneHeights>>...>;

} // detail


using all_tones_set = tone_set<
    tones::C, tones::Cs, tones::D, tones::Ds,
    tones::E, tones::F, tones::Fs,
    tones::G, tones::Gs, tones::A, tones::As,
    tones::B
>;


namespace mods {

struct none;
struct sharp;
struct flat;

struct dbl_sharp;
struct dbl_flat;

struct natural;

} // mods

template<class... Mods>
struct mod_set;



template<class Tone, class Mod>
struct ident_traits;

template<class Tone, class Mod>
struct basic_ident;

template<class... Idents>
struct ident_set;


namespace idents {

using C  = basic_ident<music::detail::make_tone<0 >, mods::none>;
using Cs = basic_ident<music::detail::make_tone<1 >, mods::sharp>;
using Cb = basic_ident<music::detail::make_tone<11>, mods::flat>;
using D  = basic_ident<music::detail::make_tone<2 >, mods::none>;
using Ds = basic_ident<music::detail::make_tone<3 >, mods::sharp>;
using Db = basic_ident<music::detail::make_tone<1 >, mods::flat>;
using E  = basic_ident<music::detail::make_tone<4 >, mods::none>;
using Es = basic_ident<music::detail::make_tone<5 >, mods::sharp>;
using Eb = basic_ident<music::detail::make_tone<3 >, mods::flat>;
using F  = basic_ident<music::detail::make_tone<5 >, mods::none>;
using Fs = basic_ident<music::detail::make_tone<6 >, mods::sharp>;
using Fb = basic_ident<music::detail::make_tone<4 >, mods::flat>;
using G  = basic_ident<music::detail::make_tone<7 >, mods::none>;
using Gs = basic_ident<music::detail::make_tone<8 >, mods::sharp>;
using Gb = basic_ident<music::detail::make_tone<6 >, mods::flat>;
using A  = basic_ident<music::detail::make_tone<9 >, mods::none>;
using As = basic_ident<music::detail::make_tone<10>, mods::sharp>;
using Ab = basic_ident<music::detail::make_tone<8 >, mods::flat>;
using B  = basic_ident<music::detail::make_tone<11>, mods::none>;
using Bs = basic_ident<music::detail::make_tone<0 >, mods::sharp>;
using Bb = basic_ident<music::detail::make_tone<10>, mods::flat>;

} // idents


namespace key_feels {

struct major;
struct minor;

} // key_feels


template<class RelativeHeight>
struct basic_degree;


namespace degrees {

using I    = basic_degree<relative_height<0>>;
using bII  = basic_degree<relative_height<1>>;
using II   = basic_degree<relative_height<2>>;
using bIII = basic_degree<relative_height<3>>;
using III  = basic_degree<relative_height<4>>;
using IV   = basic_degree<relative_height<5>>;
using sIV  = basic_degree<relative_height<6>>;
using V    = basic_degree<relative_height<7>>;
using bVI  = basic_degree<relative_height<8>>;
using VI   = basic_degree<relative_height<9>>;
using bVII = basic_degree<relative_height<10>>;
using VII  = basic_degree<relative_height<11>>;

using diatonic_seq = std::tuple<I, II, III, IV, V, VI, VII>;

} // degrees


namespace feel_mods {

struct same {};
struct opposite {};

template<class FixedKeyFeel>
struct forced_fixed { /* using fixed_type = FixedKeyFeel; */ };

} // feel_mods


template<class OriginalKeyFeel, class Degree, class FeelMod>
struct relation;


namespace relations {

template<class KeyFeel> struct tonic;
template<class KeyFeel> struct parallel;
template<class KeyFeel> struct relative;
template<class KeyFeel> struct dominant;
template<class KeyFeel> struct sub_dominant;

} // relations


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


template<class ScaledAs, class IdentSet>
struct basic_scale;

template<class ScaledAs, class... Scales>
struct dynamic_scale;

template<class Ident, class KeyFeel>
struct key_ident;

template<class KeyIdent>
struct basic_key_sign;

template<class KeyIdent>
struct key_traits;

template<class KeyIdent>
struct basic_key;


namespace keys {

template<class KeyFeel> using C = basic_key<key_ident<idents::C, KeyFeel>>;
using CMaj = C<key_feels::major>; using Cmin = C<key_feels::minor>;

template<class KeyFeel> using Cs = basic_key<key_ident<idents::Cs, KeyFeel>>;
using CsMaj = Cs<key_feels::major>; using Csmin = Cs<key_feels::minor>;

template<class KeyFeel> using Cb = basic_key<key_ident<idents::Cb, KeyFeel>>;
using CbMaj = Cb<key_feels::major>; /* using Cbmin = Cb<key_feels::minor>; */

template<class KeyFeel> using D = basic_key<key_ident<idents::D, KeyFeel>>;
using DMaj = D<key_feels::major>; using Dmin = D<key_feels::minor>;

template<class KeyFeel> using Ds = basic_key<key_ident<idents::Ds, KeyFeel>>;
/* using DsMaj = D<key_feels::major>; */ using Dsmin = Ds<key_feels::minor>;

template<class KeyFeel> using Db = basic_key<key_ident<idents::Db, KeyFeel>>;
using DbMaj = Db<key_feels::major>; /* using Dbmin = Db<key_feels::minor>; */

template<class KeyFeel> using E = basic_key<key_ident<idents::E, KeyFeel>>;
using EMaj = E<key_feels::major>; using Emin = E<key_feels::minor>;

//template<class KeyFeel> using Es = basic_key<key_ident<idents::Es, KeyFeel>>;
//using EsMaj = E<key_feels::major>; using Esmin = Es<key_feels::minor>;

template<class KeyFeel> using Eb = basic_key<key_ident<idents::Eb, KeyFeel>>;
using EbMaj = Eb<key_feels::major>; using Ebmin = Eb<key_feels::minor>;

template<class KeyFeel> using F = basic_key<key_ident<idents::F, KeyFeel>>;
using FMaj = F<key_feels::major>; using Fmin = F<key_feels::minor>;

template<class KeyFeel> using Fs = basic_key<key_ident<idents::Fs, KeyFeel>>;
using FsMaj = Fs<key_feels::major>; using Fsmin = Fs<key_feels::minor>;

//template<class KeyFeel> using Fb = basic_key<key_ident<idents::Fb, KeyFeel>>;
//using FbMaj = Fb<key_feels::major>; using Fbmin = Fb<key_feels::minor>;

template<class KeyFeel> using G = basic_key<key_ident<idents::G, KeyFeel>>;
using GMaj = G<key_feels::major>; using Gmin = G<key_feels::minor>;

template<class KeyFeel> using Gs = basic_key<key_ident<idents::Gs, KeyFeel>>;
/* using GsMaj = G<key_feels::major>; */ using Gsmin = Gs<key_feels::minor>;

template<class KeyFeel> using Gb = basic_key<key_ident<idents::Gb, KeyFeel>>;
using GbMaj = Gb<key_feels::major>; /* using Gbmin = Gb<key_feels::minor>; */

template<class KeyFeel> using A = basic_key<key_ident<idents::A, KeyFeel>>;
using AMaj = A<key_feels::major>; using Amin = A<key_feels::minor>;

template<class KeyFeel> using As = basic_key<key_ident<idents::As, KeyFeel>>;
/* using AsMaj = A<key_feels::major>; */ using Asmin = As<key_feels::minor>;

template<class KeyFeel> using Ab = basic_key<key_ident<idents::Ab, KeyFeel>>;
using AbMaj = Ab<key_feels::major>; using Abmin = Ab<key_feels::minor>;

template<class KeyFeel> using B = basic_key<key_ident<idents::B, KeyFeel>>;
using BMaj = B<key_feels::major>; using Bmin = B<key_feels::minor>;

//template<class KeyFeel> using Bs = basic_key<key_ident<idents::Bs, KeyFeel>>;
//using BsMaj = Bs<key_feels::major>; using Bsmin = Bs<key_feels::minor>;

template<class KeyFeel> using Bb = basic_key<key_ident<idents::Bb, KeyFeel>>;
using BbMaj = Bb<key_feels::major>; using Bbmin = Bb<key_feels::minor>;

} // keys


template<bool IsTension, unsigned ID, int ModOffset = 0>
struct chord_note_id;

template<class ID>
struct basic_chord_note;

template<unsigned TensionID, class Mod>
struct basic_chord_tension;


namespace chord_notes {

struct M3;
struct m3;
struct P5;
struct b5;
struct aug5;
struct M6;
struct m6;
struct M7;
struct m7;
struct dim7;
struct P9;
struct s9;
// struct b10;
struct s11;
struct P13;
struct s13;
struct b13;

} // chord_notes


template<class CN3, class CN5, class CN6, class CN7>
struct chord_fund_set;

template<class... Tensions>
struct chord_tension_set;

using no_tensions = chord_tension_set<saya::zed::void_elem>;

template<class FundSet, class TensionSet>
struct chord_traits;

template<class FundSet, class TensionSet>
struct basic_chord;


namespace contexts {

struct auto_select;

struct key;
struct scale;
struct chord;

} // contexts

}} // ompu
