#pragma once

#include "ompu/music/music_fwd.hpp"

#include <boost/variant/variant_fwd.hpp>

#include <iosfwd>
#include <stdexcept>


namespace ompu { namespace music { namespace concrete {

using octave_type = int;

struct concrete_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};


using any_mods = saya::zed::rewrap_t<
    std::tuple,
    boost::variant,
    all_mods
>;

using any_tones = saya::zed::rewrap_t<
    std::tuple,
    boost::variant,
    all_tones
>;


#if 0
// WOW.
// this won't work due to Boost.MPL limitations

using any_keys = saya::zed::rewrap_t<
    std::tuple,
    boost::variant,
    all_keys
>;
#endif


enum struct CanonicalNoteHeight : unsigned
{
    C = 0, D = 2, E = 4, F = 5, G = 7, A = 9, B = 11,
};

using note_height_type = std::underlying_type_t<CanonicalNoteHeight>;

template<class Tone, class Mod>
inline CanonicalNoteHeight make_note_height(basic_ident<Tone, Mod>)
{
    switch (Tone::height + cvt::detail::opposite_mod_t<Mod>::offset) {
    case 0:
        return CanonicalNoteHeight::C;
    case 2:
        return CanonicalNoteHeight::D;
    case 4:
        return CanonicalNoteHeight::E;
    case 5:
        return CanonicalNoteHeight::F;
    case 7:
        return CanonicalNoteHeight::G;
    case 9:
        return CanonicalNoteHeight::A;
    case 11:
        return CanonicalNoteHeight::B;

    default:
        throw std::logic_error("unhandled note height");
    }
}

std::ostream& operator<<(std::ostream& os, CanonicalNoteHeight nh);


enum struct ModType : int
{
    none = 0, natural = 1234,
    sharp = +1, dbl_sharp = +2,
    flat = -1, dbl_flat = -2,
};

inline std::underlying_type_t<ModType>
mod_value(ModType mt) noexcept
{
    if (mt == ModType::natural) return 0;
    return static_cast<std::underlying_type_t<ModType>>(mt);
}

std::ostream& operator<<(std::ostream& os, ModType mt);


template<class Mod>
inline ModType make_mod(Mod) noexcept
{
    if (std::is_same_v<Mod, mods::none>) {
        return ModType::none;
    } else if (std::is_same_v<Mod, mods::sharp>) {
        return ModType::sharp;
    } else if (std::is_same_v<Mod, mods::flat>) {
        return ModType::flat;
    } else if (std::is_same_v<Mod, mods::dbl_sharp>) {
        return ModType::dbl_sharp;
    } else if (std::is_same_v<Mod, mods::dbl_flat>) {
        return ModType::dbl_flat;
    } else if (std::is_same_v<Mod, mods::natural>) {
        return ModType::natural;
    }
}

}}} // ompu
