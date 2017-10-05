#pragma once

#include "ompu/music/key_ident.hpp"
#include "ompu/music/key_feel.hpp"
#include "ompu/music/key_traits.hpp"
#include "ompu/music/scale.hpp"
#include "ompu/music/chord.hpp"
#include "ompu/music/degree.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

template<class KeyFeel, class Degree>
struct diatonic_chords;

template<> struct diatonic_chords<key_feels::major, degrees::I>
{ using type = basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::M7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::major, degrees::II>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::major, degrees::III>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::major, degrees::IV>
{ using type = basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::M7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::major, degrees::V>
{ using type = basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::m7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::major, degrees::VI>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::major, degrees::VII>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::b5, void, chord_notes::m7>, no_tensions>; };

template<> struct diatonic_chords<key_feels::minor, degrees::I>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::M7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::minor, degrees::II>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::b5, void, chord_notes::m7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::minor, degrees::III>
{ using type = basic_chord<chord_fund_set<chord_notes::M3, chord_notes::aug5, void, chord_notes::M7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::minor, degrees::IV>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::minor, degrees::V>
{ using type = basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::m7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::minor, degrees::VI>
{ using type = basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::M7>, no_tensions>; };
template<> struct diatonic_chords<key_feels::minor, degrees::VII>
{ using type = basic_chord<chord_fund_set<chord_notes::m3, chord_notes::b5, void, chord_notes::dim7>, no_tensions>; };

template<class KeyFeel, class Degree>
using diatonic_chords_t = typename diatonic_chords<KeyFeel, Degree>::type;


template<class KeyIdent>
struct basic_key
{
    using traits_type = key_traits<KeyIdent>;
    using ident_type = typename traits_type::ident_type;
    using key_ident_type = typename traits_type::key_ident_type;
    using key_feel = typename traits_type::key_feel;
    using key_scale_type = typename traits_type::key_scale_type;
    using key_sign_type = typename traits_type::key_sign_type;

    using known_scale_roles = std::tuple<
        scales::natural<key_feel>,
        scales::harmonic<key_feel>,
        scales::melodic<key_feel>
    >;

    using diatonic_chords_seq = std::tuple<
        diatonic_chords_t<key_feel, degrees::I>,
        diatonic_chords_t<key_feel, degrees::II>,
        diatonic_chords_t<key_feel, degrees::III>,
        diatonic_chords_t<key_feel, degrees::IV>,
        diatonic_chords_t<key_feel, degrees::V>,
        diatonic_chords_t<key_feel, degrees::VI>,
        diatonic_chords_t<key_feel, degrees::VII>
    >;

    static constexpr auto height = ident_type::height;
    static constexpr auto name = ident_type::name + sprout::to_string(" ") + key_feel::name;
};

}} // ompu
