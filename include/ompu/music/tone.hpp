#pragma once

#include "ompu/music/ident.hpp"


namespace ompu { namespace music {

template<class Ident>
struct basic_tone
{
    using ident_type = Ident;
    using canonical_ident_type = detail::canonical_ident_t<Ident>;

    static constexpr bool is_canonical = std::is_same_v<ident_type, canonical_ident_type>;

    using mod_type = typename ident_type::mod_type;
};

template<class Ident>
using tone_t = basic_tone<Ident>;

template<class Tone>
inline constexpr auto
make_canonical(Tone) noexcept
{
    return typename Tone::canonical_ident_type{};
}


namespace tones {

using C = tone_t<idents::C>;
using Cs = tone_t<idents::Cs>;
using Cb = tone_t<idents::Cb>;
using D = tone_t<idents::D>;
using Ds = tone_t<idents::Ds>;
using Db = tone_t<idents::Db>;
using E = tone_t<idents::E>;
//using Es = tone_t<idents::Es>;
using Eb = tone_t<idents::Eb>;
using F = tone_t<idents::F>;
using Fs = tone_t<idents::Fs>;
using Fb = tone_t<idents::Fb>;
using G = tone_t<idents::G>;
using Gs = tone_t<idents::Gs>;
using Gb = tone_t<idents::Gb>;
using A = tone_t<idents::A>;
using As = tone_t<idents::As>;
using Ab = tone_t<idents::Ab>;
using B = tone_t<idents::B>;
//using Bs = tone_t<idents::Bs>;
using Bb = tone_t<idents::Bb>;

} // tones


template<class Tone, class ResolveToTone>
struct resolve_tone_in_context
{
    // Example:

    // Tone = C# (1, mods::None) on C#Maj
    // Resolve to Tone = D (2, mods::None) on CMaj
    // Result = C# (1, mods::Sharp) -- reason: upward resolution

    // Tone = C# (1, mods::None) on C#Maj
    // Resolve to Tone = C (0, mods::None) on CMaj
    // Result = Db (1, mods::Flat) -- reason: downward resolution

    static_assert(
        ResolveToTone::is_canonical,
        "resolution context tone must be a canonical tone (you can't resolve a tone to an already-shapred-or-flatted another tone)"
    );

private:
    static constexpr auto from_height = Tone::ident_type::height;
    static constexpr auto to_height = ResolveToTone::ident_type::height;

public:
    static_assert(
        from_height != to_height,
        "you can't resolve a tone to an another tone with same height"
    );

    using type = std::conditional_t<
        from_height < to_height,
        tone_t<tone_ident_t<from_height, mods::Sharp>>,
        tone_t<tone_ident_t<from_height, mods::Flat>>
    >;
};

template<class Tone, class ResolveToTone>
using resolve_tone_in_context_t = typename resolve_tone_in_context<Tone, ResolveToTone>::type;

}} // ompu
