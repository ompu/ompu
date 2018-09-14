#pragma once

#include "ompu/music/concrete/error.hpp"

#include <vector>
#include <bitset>
#include <type_traits>

#include <cstdint>


namespace ompu { namespace music { namespace concrete {

namespace detail {

enum class chord_spec_base : std::uint32_t;
enum class chord_spec_mix : std::underlying_type_t<chord_spec_base>;

} // detail

enum class chord_spec : std::underlying_type_t<detail::chord_spec_base>;

namespace detail {

template<class T>
inline constexpr std::underlying_type_t<chord_spec_base>
chord_spec_int(T b) { return static_cast<std::underlying_type_t<chord_spec_base>>(b); }

template<class T1, class T2>
inline constexpr std::underlying_type_t<chord_spec_base>
chord_spec_bor(T1 b1, T2 b2) { return chord_spec_int(b1) | chord_spec_int(b2); }

template<class T>
inline constexpr std::underlying_type_t<chord_spec_base>
chord_spec_bor(T b1, T b2) { return chord_spec_int(b1) | chord_spec_int(b2); }

} // detail


namespace detail {

enum class chord_spec_base : std::uint32_t
{
    _lowoct = 0b0001,
    _uppoct = 0b1000,

    _2base  = 0 + 4,
    m2      = (0b0010 | _lowoct) << _2base,
    M2      = (0b0100 | _lowoct) << _2base,
    M9      = (0b0010 | _uppoct) << _2base,
    b9      = (0b0100 | _uppoct) << _2base,
    s9      = (0b0110 | _uppoct) << _2base,

    _3base  = _2base + 4,
    m3     = ((0b0010 | _lowoct) << _3base) | (0b0110 << _2base),
    M3      = (0b0100 | _lowoct) << _3base,

    _4base  = _3base + 4,
    P4      = (0b0010 | _lowoct) << _4base,
    M11     = (0b0010 | _uppoct) << _4base,
    s11     = (0b0110 | _uppoct) << _4base,

    _5base  = _4base + 4,
    P5      = (0b0010 | _uppoct /* !!! */ ) << _5base,
    b5      = (0b0010 | _lowoct) << _5base,
    dim5    = (0b0100 | _lowoct) << _5base,
    aug5    = (0b0110 | _lowoct) << _5base,

    _6base  = _5base + 4,
    M6      = (0b0010 | _lowoct) << _6base,
    M13     = (0b0010 | _uppoct) << _6base,
    b13     = (0b0100 | _uppoct) << _6base,

    _7base  = _6base + 4,
    dim7    = (0b0010 | _lowoct) << _7base,
    m7      = (0b0100 | _lowoct) << _7base,
    M7      = (0b0110 | _lowoct) << _7base,
};

enum class chord_spec_mix : std::underlying_type_t<chord_spec_base>
{
    major_triad = chord_spec_bor(chord_spec_base::M3, chord_spec_base::P5),
    minor_triad = chord_spec_bor(chord_spec_base::m3, chord_spec_base::P5),

    dim = chord_spec_bor(chord_spec_base::m3, chord_spec_base::dim5),
    aug = chord_spec_bor(chord_spec_base::M3, chord_spec_base::aug5),
};

enum class chord_spec : std::underlying_type_t<detail::chord_spec_base>
{
    power       = chord_spec_int(chord_spec_base::P5),
    _5          = chord_spec_bor(chord_spec_base::M3, chord_spec_base::b5),
    aug         = chord_spec_int(chord_spec_mix::aug),

    add2        = chord_spec_bor(chord_spec_mix::major_triad, chord_spec_base::M2),
    add9        = chord_spec_bor(chord_spec_mix::major_triad, chord_spec_base::M9),
    sus2        = chord_spec_bor(chord_spec_base::M2, chord_spec_base::P5),
    sus4        = chord_spec_bor(chord_spec_base::P4, chord_spec_base::P5),
    add4        = chord_spec_bor(chord_spec_mix::major_triad, chord_spec_base::P4),

    _6          = chord_spec_bor(chord_spec_mix::major_triad, chord_spec_base::M6),
    _69         = chord_spec_bor(_6, chord_spec_base::M9),

    _7          = chord_spec_bor(chord_spec_mix::major_triad, chord_spec_base::m7),
    dim7        = chord_spec_bor(chord_spec_mix::dim, chord_spec_base::dim7),
    m7          = chord_spec_bor(chord_spec_mix::minor_triad, chord_spec_base::m7),
    M7          = chord_spec_bor(chord_spec_mix::major_triad, chord_spec_base::M7),
    mM7         = chord_spec_bor(chord_spec_mix::minor_triad, chord_spec_base::M7),
    m7b5        = chord_spec_bor(chord_spec_mix::dim, chord_spec_base::m7),

    _9          = chord_spec_bor(_7, chord_spec_base::M9),
};

enum class chord_tension_spec : std::underlying_type_t<detail::chord_spec_base>
{
    add9        = chord_spec_base::M9,
    b9          = 0b0011 << 4,
    s9          = 0b0111 << 4,

    s11         = 0b0011 << 8,
};

} // detail


using chord_spec = detail::chord_spec;
using chord_tension_spec = detail::chord_tension_spec;


class ChordIdentity
{
public:
    ChordIdentity() = default;
    ChordIdentity(ChordIdentity const&) = default;
    ChordIdentity(ChordIdentity&&) noexcept = default;

    ChordIdentity& operator=(ChordIdentity const&) = default;
    ChordIdentity& operator=(ChordIdentity&&) noexcept = default;

    ~ChordIdentity() = default;

    void force(forced_chord_spec spec);
    void add(added_chord_spec spec);

private:
    std::bitset<32> specs_;
};

}}} // ompu
