#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/ident.hpp"


namespace ompu { namespace music {

template<class Ident>
struct basic_tone
{
    using ident_type = Ident;
    using canonical_ident_type = cvt::detail::canonical_ident_t<ident_type>;
    using canonical_tone_type = basic_tone<canonical_ident_type>;
    using mod_type = typename ident_type::mod_type;
    static constexpr auto height = ident_type::height;

    static constexpr bool is_canonical = std::is_same_v<basic_tone, canonical_tone_type>;
    static constexpr auto name = ident_type::name;
};


namespace tones {

using C = basic_tone<idents::C>;
using Cs = basic_tone<idents::Cs>;
using Cb = basic_tone<idents::Cb>;
using D = basic_tone<idents::D>;
using Ds = basic_tone<idents::Ds>;
using Db = basic_tone<idents::Db>;
using E = basic_tone<idents::E>;
//using Es = basic_tone<idents::Es>;
using Eb = basic_tone<idents::Eb>;
using F = basic_tone<idents::F>;
using Fs = basic_tone<idents::Fs>;
using Fb = basic_tone<idents::Fb>;
using G = basic_tone<idents::G>;
using Gs = basic_tone<idents::Gs>;
using Gb = basic_tone<idents::Gb>;
using A = basic_tone<idents::A>;
using As = basic_tone<idents::As>;
using Ab = basic_tone<idents::Ab>;
using B = basic_tone<idents::B>;
//using Bs = basic_tone<idents::Bs>;
using Bb = basic_tone<idents::Bb>;

} // tones

}} // ompu
