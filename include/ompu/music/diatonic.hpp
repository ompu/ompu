#pragma once

#include "ompu/music/degree.hpp"
#include "ompu/music/degreed_chord.hpp"


namespace ompu { namespace music {

template<class KeyFeel, class Degree, bool IsTetrad>
struct diatonic_chords;

template<> struct diatonic_chords<key_feels::major, degrees::I, true>
{ using type = degreed_chord<degrees::I, basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::M7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::major, degrees::II, true>
{ using type = degreed_chord<degrees::II, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::major, degrees::III, true>
{ using type = degreed_chord<degrees::III, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::major, degrees::IV, true>
{ using type = degreed_chord<degrees::IV, basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::M7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::major, degrees::V, true>
{ using type = degreed_chord<degrees::V, basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::m7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::major, degrees::VI, true>
{ using type = degreed_chord<degrees::VI, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::major, degrees::VII, true>
{ using type = degreed_chord<degrees::VII, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::b5, void, chord_notes::m7>, chord_tension_set<>>>; };

template<> struct diatonic_chords<key_feels::minor, degrees::I, true>
{ using type = degreed_chord<degrees::I, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::M7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::minor, degrees::II, true>
{ using type = degreed_chord<degrees::II, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::b5, void, chord_notes::m7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::minor, degrees::III, true>
{ using type = degreed_chord<degrees::III, basic_chord<chord_fund_set<chord_notes::M3, chord_notes::aug5, void, chord_notes::M7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::minor, degrees::IV, true>
{ using type = degreed_chord<degrees::IV, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::P5, void, chord_notes::m7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::minor, degrees::V, true>
{ using type = degreed_chord<degrees::V, basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::m7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::minor, degrees::VI, true>
{ using type = degreed_chord<degrees::VI, basic_chord<chord_fund_set<chord_notes::M3, chord_notes::P5, void, chord_notes::M7>, chord_tension_set<>>>; };
template<> struct diatonic_chords<key_feels::minor, degrees::VII, true>
{ using type = degreed_chord<degrees::VII, basic_chord<chord_fund_set<chord_notes::m3, chord_notes::b5, void, chord_notes::dim7>, chord_tension_set<>>>; };


template<class KeyFeel, class Degree>
struct diatonic_chords<
    KeyFeel, Degree, false
>
{
    using type = degreed_chord<
        Degree,
        cvt::reduce_to_triad_t<typename diatonic_chords<KeyFeel, Degree, true>::type::chord_type>
    >;
};

template<class KeyFeel, class Degree, bool IsTetrad>
using diatonic_chords_t = typename diatonic_chords<KeyFeel, Degree, IsTetrad>::type;


template<class KeyFeel, class Degree>
struct diatonic_chords_tetrad_helper
{
    using type = typename diatonic_chords<KeyFeel, Degree, true>::type;
};

template<class KeyFeel, class Degree>
struct diatonic_chords_triad_helper
{
    using type = typename diatonic_chords<KeyFeel, Degree, false>::type;
};

}} // ompu
