#pragma once

#include "ompu/music/traits.hpp"

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/subseq.hpp>
#include <boost/preprocessor/seq/elem.hpp>

#include <type_traits>


namespace ompu { namespace music {

template<class Traits_ = Traits<>>
class Note
{
public:
    static constexpr char const* Names[] = {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };

    explicit Note(unsigned n)
        : Num(n)
        , Name(Names[n % 12])
        , Freq(std::pow(2, (Num - 69) / 12.) * Traits_::A4_Freq)
    {}

    unsigned const Num;
    char const* const Name;
    double const Freq;
};


namespace notes {

struct MIDI {};
struct ClosestFreq {};
struct Absolute {};
struct Offset {};


#define OMPU_NOTE_NAMES_DEF \
    (C)(D)(E)(F)(G)(A)(B)

#define OMPU_DEF(z, n, d) \
    struct BOOST_PP_SEQ_ELEM(n, d) : std::integral_constant<unsigned, BOOST_PP_MUL(n, 2)> {};

BOOST_PP_REPEAT(7, OMPU_DEF, OMPU_NOTE_NAMES_DEF)
#undef OMPU_DEF

#define OMPU_DEF_I(n, name) \
    struct BOOST_PP_CAT(name, s) : std::integral_constant<unsigned, n> {};

#define OMPU_DEF(z, n, d) \
    OMPU_DEF_I(BOOST_PP_ADD(BOOST_PP_MUL(n, 2), 1), BOOST_PP_SEQ_ELEM(n, d))

BOOST_PP_REPEAT(6, OMPU_DEF, BOOST_PP_SEQ_SUBSEQ(OMPU_NOTE_NAMES_DEF, 0, 6))
OMPU_DEF_I(0, B)

#undef OMPU_DEF_I
#undef OMPU_DEF

#define OMPU_DEF_I(n, name) \
    struct BOOST_PP_CAT(name, b) : std::integral_constant<unsigned, n> {};

#define OMPU_DEF(z, n, d) \
    OMPU_DEF_I(BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(n, 1), 2), 1), BOOST_PP_SEQ_ELEM(n, d))

BOOST_PP_REPEAT(6, OMPU_DEF, BOOST_PP_SEQ_SUBSEQ(OMPU_NOTE_NAMES_DEF, 1, 6))
OMPU_DEF_I(12, C)

#undef OMPU_DEF_I
#undef OMPU_DEF

} // notes

}} // ompu
