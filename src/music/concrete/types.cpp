#include "ompu/music/concrete/types.hpp"

#include "ompu/music/detail/symbol_names.hpp"


namespace ompu { namespace music { namespace concrete {

std::ostream& operator<<(std::ostream& os, CanonicalNoteHeight nh)
{
    using NH = CanonicalNoteHeight;
    switch (nh) {
    case NH::C: return os << "C";
    case NH::D: return os << "D";
    case NH::E: return os << "E";
    case NH::F: return os << "F";
    case NH::G: return os << "G";
    case NH::A: return os << "A";
    case NH::B: return os << "B";
    default: throw std::logic_error("unhandled CanonicalNoteHeight value");
    }
}

std::ostream& operator<<(std::ostream& os, ModType mt)
{
    using MT = ModType;
    using music::detail::symbol_names;

    switch (mt) {
    case MT::none: return os;
    case MT::sharp: return os << symbol_names::sharp;
    case MT::flat: return os << symbol_names::flat;
    case MT::dbl_sharp: return os << symbol_names::sharp << symbol_names::sharp;
    case MT::dbl_flat: return os << symbol_names::flat << symbol_names::flat;
    case MT::natural: return os << symbol_names::natural;
    default: throw std::logic_error("unhandled ModType value");
    }
}

}}} // ompu
