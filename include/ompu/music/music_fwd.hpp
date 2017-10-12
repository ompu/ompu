#pragma once

#include "ompu/config.hpp"

#pragma warning(push)
#if BOOST_MSVC
#pragma warning(disable: 4100)
#endif

#include "sprout/string.hpp"
#pragma warning(pop)

#include "ompu/music/detail/symbol_names.hpp"

#include "saya/zed/meta.hpp"

#include <tuple>
#include <type_traits>
#include <utility>


namespace ompu { namespace music {

template<unsigned Height>
struct tone_height;

template<int Offset>
struct tone_offset;

template<class ToneHeight>
struct basic_tone;


namespace detail {

using all_heights = std::tuple<
    tone_height<0>, tone_height<1>, tone_height<2>,
    tone_height<3>, tone_height<4>, tone_height<5>,
    tone_height<6>, tone_height<7>, tone_height<8>,
    tone_height<9>, tone_height<10>, tone_height<11>
>;

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

using all_tones = std::tuple<
    tones::C, tones::Cs, tones::D, tones::Ds, tones::E,
    tones::F, tones::Fs, tones::G, tones::Gs, tones::A, tones::As, tones::B
>;


template<class... Tones>
struct tone_set;

namespace detail {

template<unsigned ... ToneHeights>
using make_tone_set = tone_set<basic_tone<tone_height<ToneHeights>>...>;

namespace predef {

using C_natural_major_tone_set = make_tone_set<0, 2, 4, 5, 7, 9, 11>;
using A_natural_minor_tone_set = make_tone_set<9, 11, 0, 2, 4, 5, 7>;

using C_natural_minor_tone_set = make_tone_set<0, 2, 3, 5, 7, 8, 10>;

} // predef

} // detail


namespace mods {

struct none;
struct sharp;
struct flat;

struct dbl_sharp;
struct dbl_flat;

struct natural;

} // mods

using all_mods = std::tuple<
    mods::none, mods::sharp, mods::dbl_sharp, mods::flat, mods::dbl_flat, mods::natural
>;

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

using all_idents = saya::zed::zipped_t<
    std::tuple,
    basic_ident,
    all_tones,
    all_mods
>;


namespace key_feels {

struct major;
struct minor;

} // key_feels

using all_key_feels = std::tuple<key_feels::major, key_feels::minor>;


template<unsigned N>
struct degree_height;

template<class DegreeHeight, class Mod = mods::none>
struct basic_degree;

template<class Degree, class RelativeToneHeight>
struct degreed_tone;

template<class Degree, class Chord>
struct degreed_chord;


namespace degrees {

using I    = basic_degree<degree_height<1>>;
using bII  = basic_degree<degree_height<2>, mods::flat>;
using II   = basic_degree<degree_height<2>>;
using bIII = basic_degree<degree_height<3>, mods::flat>;
using III  = basic_degree<degree_height<3>>;
using IV   = basic_degree<degree_height<4>>;
using sIV  = basic_degree<degree_height<4>, mods::sharp>;
using V    = basic_degree<degree_height<5>>;
using bVI  = basic_degree<degree_height<6>, mods::flat>;
using VI   = basic_degree<degree_height<6>>;
using bVII = basic_degree<degree_height<7>, mods::flat>;
using VII  = basic_degree<degree_height<7>>;

} // degrees

using all_diatonic_degrees = std::tuple<degrees::I, degrees::II, degrees::III, degrees::IV, degrees::V, degrees::VI, degrees::VII>;
using all_degrees = std::tuple<degrees::I, degrees::bII, degrees::II, degrees::bIII, degrees::III, degrees::IV, degrees::sIV, degrees::V, degrees::bVI, degrees::VI, degrees::bVII, degrees::VII>;


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
struct key_natural_tones;

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

using all_keys = std::tuple<
    keys::CMaj , keys::Cmin ,
    keys::CsMaj, keys::Csmin,
    keys::CbMaj,
    keys::DMaj , keys::Dmin ,
    keys::Dsmin,
    keys::DbMaj,
    keys::EMaj , keys::Emin ,
    keys::EbMaj, keys::Ebmin,
    keys::FMaj , keys::Fmin ,
    keys::FsMaj, keys::Fsmin,
    keys::GMaj , keys::Gmin ,
    keys::Gsmin,
    keys::GbMaj,
    keys::AMaj , keys::Amin ,
    keys::Asmin,
    keys::AbMaj, keys::Abmin,
    keys::BMaj , keys::Bmin ,
    keys::BbMaj, keys::Bbmin
>;


template<bool IsTension, unsigned ID, class Mod = mods::none>
struct chord_note_id;

template<class ID>
struct basic_chord_note;


namespace chord_notes {

using M3   = basic_chord_note<chord_note_id<false, 3>>;
using m3   = basic_chord_note<chord_note_id<false, 3, mods::flat>>;
using P5   = basic_chord_note<chord_note_id<false, 5>>;
using b5   = basic_chord_note<chord_note_id<false, 5, mods::flat>>;
using aug5 = basic_chord_note<chord_note_id<false, 5, mods::sharp>>;
using dim5 = basic_chord_note<chord_note_id<false, 5, mods::dbl_flat>>;
using P6   = basic_chord_note<chord_note_id<false, 6>>;
using M7   = basic_chord_note<chord_note_id<false, 7>>;
using m7   = basic_chord_note<chord_note_id<false, 7, mods::flat>>;
using dim7 = basic_chord_note<chord_note_id<false, 7, mods::dbl_flat>>;

using P9   = basic_chord_note<chord_note_id<true, 9>>;
using s9   = basic_chord_note<chord_note_id<true, 9, mods::sharp>>;
using s11  = basic_chord_note<chord_note_id<true, 11, mods::sharp>>;
using P13  = basic_chord_note<chord_note_id<true, 13>>;
using s13  = basic_chord_note<chord_note_id<true, 13, mods::sharp>>;
using b13  = basic_chord_note<chord_note_id<true, 13, mods::flat>>;

} // chord_notes


template<class CN3, class CN5, class CN6, class CN7>
struct chord_fund_set;

template<class... Tensions>
struct chord_tension_set;

template<class FundSet, class TensionSet>
struct chord_traits;

template<class FundSet, class TensionSet>
struct basic_chord;

template<class Degree, class Chord>
struct degreed_chord;


namespace contexts {

struct auto_select;

struct key;
struct scale;
struct chord;

} // contexts

}} // ompu
