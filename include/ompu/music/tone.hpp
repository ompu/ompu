#pragma once

#include "ompu/music/ident.hpp"


namespace ompu { namespace music {

template<class Ident>
struct basic_tone
{
    using ident_type = Ident;
    using mod_type = typename ident_type::mod_type;
};


namespace detail {

template<class Ident>
struct make_tone_impl
{
    using type = basic_tone<Ident>;
};

template<unsigned Root, class Mod>
struct make_tone_impl<make_basic_tone_ident<Root, Mod>>
{
    using type = basic_tone<canonical_ident_t<make_basic_tone_ident<Root, Mod>>>;
};

} // detail

template<class... Args>
using make_tone = typename detail::make_tone_impl<Args...>::type;


namespace tones {

using C = make_tone<idents::C>;
using Cs = make_tone<idents::Cs>;
using Cb = make_tone<idents::Cb>;
using D = make_tone<idents::D>;
//using Ds = make_tone<idents::Ds>;
using Db = make_tone<idents::Db>;
using E = make_tone<idents::E>;
//using Es = make_tone<idents::Es>;
using Eb = make_tone<idents::Eb>;
using F = make_tone<idents::F>;
using Fs = make_tone<idents::Fs>;
//using Fb = make_tone<idents::Fb>;
using G = make_tone<idents::G>;
//using Gs = make_tone<idents::Gs>;
using Gb = make_tone<idents::Gb>;
using A = make_tone<idents::A>;
//using As = make_tone<idents::As>;
using Ab = make_tone<idents::Ab>;
using B = make_tone<idents::B>;
//using Bs = make_tone<idents::Bs>;
using Bb = make_tone<idents::Bb>;

} // tones

}} // ompu
