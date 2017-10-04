#pragma once

#include "ompu/music/music_fwd.hpp"

#include "saya/zed/seq.hpp"
#include "saya/zed/fold.hpp"
#include "saya/zed/meta.hpp"
#include "saya/zed/blackhole.hpp"

#include <type_traits>
#include <tuple>
#include <utility>


namespace ompu { namespace music {

namespace cvt {

template<class Seq>
struct to_tuple;

template<class... Args>
struct to_tuple<std::tuple<Args...>>
{
    using type = std::tuple<Args...>;
};

template<class... Idents>
struct to_tuple<ident_set<Idents...>>
{
    using type = std::tuple<Idents...>;
};

template<class... Tones>
struct to_tuple<tone_set<Tones...>>
{
    using type = std::tuple<Tones...>;
};

template<class Seq>
using to_tuple_t = typename to_tuple<Seq>::type;


template<class T, class Enabled = void>
struct canonical;

template<class T>
using canonical_t = typename canonical<T>::type;

} // cvt

template<class T, class Enabled = void>
struct is_canonical : std::is_same<T, cvt::canonical_t<T>> {};

template<class T>
static constexpr bool is_canonical_v = is_canonical<T>::value;


namespace cvt {

namespace detail {

template<class Mod> struct opposite_mod;
template<> struct opposite_mod<mods::none> { using type = mods::none; };
template<> struct opposite_mod<mods::sharp> { using type = mods::flat; };
template<> struct opposite_mod<mods::flat> { using type = mods::sharp; };
template<> struct opposite_mod<mods::dbl_sharp> { using type = mods::dbl_flat; };
template<> struct opposite_mod<mods::dbl_flat> { using type = mods::dbl_sharp; };
template<> struct opposite_mod<mods::natural> { using type = mods::natural; };

template<class Mod> using opposite_mod_t = typename opposite_mod<Mod>::type;


template<int Ofs>
struct height_shift_canonical_offset
    : std::conditional_t<
        Ofs < 0,
        std::integral_constant<int, -(-Ofs % 12)>,
        std::integral_constant<int, Ofs % 12>
    >
{};

template<ident_height_type Height, int O, int Delta = int(Height) + height_shift_canonical_offset<O>::value>
struct height_shift_impl
    : std::conditional_t<
        Delta < 0,
        std::integral_constant<ident_height_type, 12 + Delta>,
        std::integral_constant<ident_height_type, Delta % 12>
    >
{};

template<ident_height_type Height, int Ofs>
struct height_shift : height_shift_impl<Height, Ofs> {};

template<ident_height_type Height, int Ofs>
constexpr auto height_shift_v = height_shift<Height, Ofs>::value;


template<class Ident>
struct add_sharp;

template<ident_height_type Height>
struct add_sharp<basic_ident<Height, mods::none>>
{
    using type = basic_ident<Height + 1, mods::sharp>;
};
template<ident_height_type Height>
struct add_sharp<basic_ident<Height, mods::sharp>>
{
    using type = basic_ident<Height + 1, mods::dbl_sharp>;
};
template<ident_height_type Height>
struct add_sharp<basic_ident<Height, mods::flat>>
{
    using type = basic_ident<Height + 1, mods::natural>;
};
template<class Ident>
using add_sharp_t = typename add_sharp<Ident>::type;


template<class Ident>
struct add_flat;

template<ident_height_type Height>
struct add_flat<basic_ident<Height, mods::none>>
{
    using type = basic_ident<Height - 1, mods::flat>;
};
template<ident_height_type Height>
struct add_flat<basic_ident<Height, mods::sharp>>
{
    using type = basic_ident<Height - 1, mods::natural>;
};
template<ident_height_type Height>
struct add_flat<basic_ident<Height, mods::flat>>
{
    using type = basic_ident<Height - 1, mods::dbl_flat>;
};
template<class Ident>
using add_flat_t = typename add_flat<Ident>::type;

} // cvt::detail


template<class Mod, class AppliedMod>
struct add_mod;

template<class AppliedMod>
struct add_mod<mods::none, AppliedMod> { using type = AppliedMod; };
template<> struct add_mod<mods::sharp, mods::sharp> { using type = mods::dbl_sharp; };
template<> struct add_mod<mods::sharp, mods::flat> { using type = mods::natural; };
template<> struct add_mod<mods::flat, mods::flat> { using type = mods::dbl_flat; };
template<> struct add_mod<mods::flat, mods::sharp> { using type = mods::natural; };

template<class Mod, class AppliedMod>
using add_mod_t = typename add_mod<Mod, AppliedMod>::type;


namespace detail {

template<class Ident, class Mod>
struct modded_ident;

template<class Ident>
struct modded_ident<Ident, mods::none>
{
    using type = Ident;
};

template<class Ident>
struct modded_ident<Ident, mods::sharp>
{
    using type = detail::add_sharp_t<Ident>;
};

template<class Ident>
struct modded_ident<Ident, mods::flat>
{
    using type = detail::add_flat_t<Ident>;
};

template<class Ident, class Mod>
using modded_ident_t = typename modded_ident<Ident, Mod>::type;


// FIXME
#if 0

template<class T>
struct strict_root_holder { /* using type = T; */ };

template<class Ident, class RootIdent>
struct rooted_ident;

template<class Ident, class RootIdent>
struct rooted_ident<Ident, strict_root_holder<RootIdent>>
{
    using type =
};
#endif

} // cvt::detail

template<class Ident> struct canonical_ident;
template<class Mod> struct canonical_ident<basic_ident<0, Mod>> { using type = idents::C; };
template<class Mod> struct canonical_ident<basic_ident<1, Mod>> { using type = idents::Cs; };
template<class Mod> struct canonical_ident<basic_ident<2, Mod>> { using type = idents::D; };
template<class Mod> struct canonical_ident<basic_ident<3, Mod>> { using type = idents::Ds; };
template<class Mod> struct canonical_ident<basic_ident<4, Mod>> { using type = idents::E; };
template<class Mod> struct canonical_ident<basic_ident<5, Mod>> { using type = idents::F; };
template<class Mod> struct canonical_ident<basic_ident<6, Mod>> { using type = idents::Fs; };
template<class Mod> struct canonical_ident<basic_ident<7, Mod>> { using type = idents::G; };
template<class Mod> struct canonical_ident<basic_ident<8, Mod>> { using type = idents::Gs; };
template<class Mod> struct canonical_ident<basic_ident<9, Mod>> { using type = idents::A; };
template<class Mod> struct canonical_ident<basic_ident<10, Mod>> { using type = idents::As; };
template<class Mod> struct canonical_ident<basic_ident<11, Mod>> { using type = idents::B; };

template<class Ident>
using canonical_ident_t = typename canonical_ident<Ident>::type;


template<ident_height_type Height, class Mod>
struct canonical<basic_ident<Height, Mod>>
{
    using type = canonical_ident_t<basic_ident<Height, Mod>>;
};


namespace detail {

template<ident_height_type Height> struct canonical_ident_height_impl;
template<> struct canonical_ident_height_impl<0> : std::integral_constant<ident_height_type, 0> {};
template<> struct canonical_ident_height_impl<1> : std::integral_constant<ident_height_type, 0> {};
template<> struct canonical_ident_height_impl<2> : std::integral_constant<ident_height_type, 1> {};
template<> struct canonical_ident_height_impl<3> : std::integral_constant<ident_height_type, 1> {};
template<> struct canonical_ident_height_impl<4> : std::integral_constant<ident_height_type, 2> {};
template<> struct canonical_ident_height_impl<5> : std::integral_constant<ident_height_type, 3> {};
template<> struct canonical_ident_height_impl<6> : std::integral_constant<ident_height_type, 3> {};
template<> struct canonical_ident_height_impl<7> : std::integral_constant<ident_height_type, 4> {};
template<> struct canonical_ident_height_impl<8> : std::integral_constant<ident_height_type, 4> {};
template<> struct canonical_ident_height_impl<9> : std::integral_constant<ident_height_type, 5> {};
template<> struct canonical_ident_height_impl<10> : std::integral_constant<ident_height_type, 5> {};
template<> struct canonical_ident_height_impl<11> : std::integral_constant<ident_height_type, 6> {};


template<ident_height_type Height, class Mod>
struct canonical_ident_height : std::integral_constant<
    ident_height_type,
    canonical_ident_height_impl<
        height_shift_v<
            Height, opposite_mod_t<Mod>::value
        >
    >::value
>
{};

template<ident_height_type Height, class Mod>
constexpr auto canonical_ident_height_v = canonical_ident_height<Height, Mod>::value;

} // cvt::detail



template<class Tone>
struct canonical_tone;

template<class Ident>
struct canonical_tone<basic_tone<Ident>>
{
    using type = basic_tone<canonical_ident_t<Ident>>;
};
template<class Tone>
using canonical_tone_t = typename canonical_tone<Tone>::type;


template<class... Ts>
inline constexpr auto
make_canonical(basic_tone<Ts...>) noexcept
{
    return canonical_tone_t<basic_tone<Ts...>>{};
}

template<class... Ts>
struct canonical<basic_tone<Ts...>>
{
    using type = canonical_tone_t<basic_tone<Ts...>>;
};


template<class Scale>
struct canonical_scale;

template<class OriginallyScaledAs, class... Tones>
struct canonical_scale<basic_scale<OriginallyScaledAs, tone_set<Tones...>>>
{
    // short-circuit for originally-canonically-defined scale
    using type = std::conditional_t<
        std::is_same_v<OriginallyScaledAs, scales::canonical>,
        basic_scale<OriginallyScaledAs, tone_set<Tones...>>,
        basic_scale<
            scales::canonical,
            tone_set<canonical_tone_t<Tones>...>
        >
    >;
};

template<class Scale>
using canonical_scale_t = typename canonical_scale<Scale>::type;

template<class... Ts>
struct canonical<basic_scale<Ts...>>
{
    using type = canonical_scale_t<basic_scale<Ts...>>;
};


namespace detail {

template<class HeightSet, int Offset>
struct height_set_shift;

template<ident_height_type... Heights, int Offset>
struct height_set_shift<height_set<Heights...>, Offset>
{
    using type = height_set<
        height_shift<Heights, Offset>::value...
    >;
};

template<class HeightSet, int Offset>
using height_set_shift_t = typename height_set_shift<HeightSet, Offset>::type;

} // detail

} // cvt


namespace detail {

template<ident_height_type Height, class HeightSet>
struct is_non_modded_height_impl;

template<ident_height_type Height, ident_height_type... CompareToHeight>
struct is_non_modded_height_impl<
    Height,
    height_set<CompareToHeight...>
> : saya::zed::any_of<
    saya::zed::eq,
    saya::zed::template_<std::integral_constant<ident_height_type, Height>, saya::zed::meta_arg>,
    std::integral_constant<ident_height_type, CompareToHeight>...
>
{};

template<ident_height_type Height>
struct is_non_modded_height
    : is_non_modded_height_impl<Height, height_set<0, 2, 4, 5, 7, 9, 11>>
{};

template<ident_height_type Height>
constexpr bool is_non_modded_height_v = is_non_modded_height<Height>::value;


template<class HeightSet>
struct modded_height_count;

template<ident_height_type... Heights>
struct modded_height_count<
    height_set<Heights...>
> : std::integral_constant<
    std::size_t,
    saya::zed::fold_add_v<
        std::size_t,
        0,
        std::conditional_t<
            is_non_modded_height<Heights>::value,
            std::integral_constant<std::size_t, 0>,
            std::integral_constant<std::size_t, 1>
        >::value...
    >
>
{};

template<class HeightSet>
constexpr auto modded_height_count_v = modded_height_count<HeightSet>::value;

} // detail


template<class Tone, class Scale>
struct is_off_scaled;

template<class Ident, class ScaledAs, class... Tones>
struct is_off_scaled<
    basic_tone<Ident>, basic_scale<ScaledAs, tone_set<Tones...>>
> : saya::zed::any_of<
    saya::zed::eq,
    saya::zed::template_<Ident, saya::zed::meta_arg>,
    typename Tones::ident_type...
>
{
    static_assert(
        !std::is_same_v<ScaledAs, scales::wild>,
        "it does not make sense to check whether a tone is off-scaled in a wild scale"
    );
};

template<class Tone, class Scale>
constexpr bool is_off_scaled_v = is_off_scaled<Tone, Scale>;


template<class ToneSet, class CompareToScale>
struct off_scaled_tone_count;

template<class CompareToScale, class... Tones>
struct off_scaled_tone_count<
    tone_set<Tones...>,
    CompareToScale
> : std::integral_constant<std::size_t, (0 + is_off_scaled<Tones, CompareToScale>::value)...>
{};

template<class ToneSet, class CompareToScale>
constexpr auto off_scaled_tone_count_v = off_scaled_tone_count<ToneSet, CompareToScale>::value;


namespace cvt {
namespace detail {

template<ident_height_type Height, class Mod>
struct mod_if_off_scaled
{
    using type = basic_ident<
        Height,
        std::conditional_t<
            music::detail::is_non_modded_height_v<Height>,
            mods::none,
            Mod
        >
    >;
};
template<ident_height_type Height, class Mod>
using mod_if_off_scaled_t = typename mod_if_off_scaled<Height, Mod>::type;

} // cvt::detail

} // cvt



template<ident_height_type Height, class KeyFeel>
struct key_sign_mod_count;

template<ident_height_type Height>
struct key_sign_mod_count<Height, key_feels::major>
    : std::integral_constant<
        std::size_t,
        detail::modded_height_count_v<
            cvt::detail::height_set_shift_t<
                height_set<0, 2, 4, 5, 7, 9, 11>,
                Height
            >
        >
    >
{};

template<ident_height_type Height>
struct key_sign_mod_count<Height, key_feels::minor>
    : std::integral_constant<
        std::size_t,
        key_sign_mod_count<cvt::detail::height_shift_v<Height, -9>, key_feels::major>::value
    >
{};

template<ident_height_type Height, class KeyFeel>
constexpr auto key_sign_mod_count_v = key_sign_mod_count<Height, KeyFeel>::value;


template<class KeyIdent> struct key_sign_mod;
template<> struct key_sign_mod<key_ident<idents::C, key_feels::major>> { using type = mods::none; };
template<> struct key_sign_mod<key_ident<idents::A, key_feels::minor>> { using type = mods::none; };

template<> struct key_sign_mod<key_ident<idents::G, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::E, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::D, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::B, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::A, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Fs, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::E, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Cs, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::B, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Gs, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Fs, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Ds, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Cs, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::As, key_feels::minor>> { using type = mods::sharp; };

template<> struct key_sign_mod<key_ident<idents::F, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::D, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Bb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::G, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Eb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::C, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Ab, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::F, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Db, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Bb, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Gb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Eb, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Cb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Ab, key_feels::minor>> { using type = mods::flat; };

template<class KeyIdent>
using key_sign_mod_t = typename key_sign_mod<KeyIdent>::type;


namespace cvt {

template<class KeyIdent, class Target>
struct interpret_in_key;

template<class KeyIdent, ident_height_type Height, class Mod>
struct interpret_in_key<KeyIdent, basic_ident<Height, Mod>>
{
    using type = detail::mod_if_off_scaled_t<
        cvt::detail::height_shift_v<
            Height,
            std::conditional_t<
                std::is_same_v<typename KeyIdent::key_feel, key_feels::major>,
                std::integral_constant<ident_height_type, KeyIdent::ident_type::height>,
                std::integral_constant<ident_height_type, cvt::detail::height_shift_v<KeyIdent::ident_type::height, -9>>
            >::value
        >,
        key_sign_mod_t<KeyIdent>
    >;
};

template<class KeyIdent, class Ident>
struct interpret_in_key<KeyIdent, basic_tone<Ident>>
{
    using type = basic_tone<typename interpret_in_key<KeyIdent, Ident>::type>;
};

template<class KeyIdent, class... Tones>
struct interpret_in_key<KeyIdent, tone_set<Tones...>>
{
    using type = tone_set<typename interpret_in_key<KeyIdent, Tones>::type...>;
};

template<class KeyIdent, class Target>
using interpret_in_key_t = typename interpret_in_key<KeyIdent, Target>::type;


} // cvt


namespace cvt { namespace detail {

template<class Key>
struct force_enharmonic_key;

template<class Ident, class KeyFeel>
struct force_enharmonic_key<basic_key<key_ident<Ident, KeyFeel>>>
{
    using type = basic_key<key_ident<canonical_ident_t<Ident>, KeyFeel>>;
};

template<class Key>
using force_enharmonic_key_t = typename force_enharmonic_key<Key>::type;

} // detail


template<bool IsGoingUp, class Scale>
struct resolve_dynamic_scale;

template<bool IsGoingUp, class Scale>
struct resolve_dynamic_scale<IsGoingUp, dynamic_scale<Scale>>
{
    using type = typename dynamic_scale<Scale>::scale_type;
};

template<class Upward, class Downward>
struct resolve_dynamic_scale<true, dynamic_scale<Upward, Downward>>
{
    using type = typename dynamic_scale<Upward, Downward>::upward_scale_type;
};

template<class Upward, class Downward>
struct resolve_dynamic_scale<false, dynamic_scale<Upward, Downward>>
{
    using type = typename dynamic_scale<Upward, Downward>::downward_scale_type;
};

template<bool IsGoingUp, class... Scales>
using resolve_dynamic_scale_t = typename resolve_dynamic_scale<IsGoingUp, Scales...>::type;

template<class Scale>
inline /* constexpr */ auto make_resolve_dynamic_scale(Scale const&, bool const is_going_up)
{
    if (is_going_up) {
        return resolve_dynamic_scale_t<true, Scale>{};
    } else {
        return resolve_dynamic_scale_t<false, Scale>{};
    }
}


template<class... Scales>
using canonical_key_scale_t = typename resolve_dynamic_scale<true, Scales...>::type;

} // cvt


namespace detail {

template<class Ident> struct sharp_counter : std::integral_constant<ident_height_type, 0> {};
template<ident_height_type Height> struct sharp_counter<basic_ident<Height, mods::sharp>> : std::integral_constant<ident_height_type, 1> {};
template<ident_height_type Height> struct sharp_counter<basic_ident<Height, mods::dbl_sharp>> : std::integral_constant<ident_height_type, 2> {};
template<class Ident> constexpr auto sharp_counter_v = sharp_counter<Ident>::value;

template<class Ident> struct flat_counter : std::integral_constant<ident_height_type, 0> {};
template<ident_height_type Height> struct flat_counter<basic_ident<Height, mods::flat>> : std::integral_constant<ident_height_type, 1> {};
template<ident_height_type Height> struct flat_counter<basic_ident<Height, mods::dbl_flat>> : std::integral_constant<ident_height_type, 2> {};
template<class Ident> constexpr auto flat_counter_v = flat_counter<Ident>::value;



template<class Key>
struct has_enharmonic_key_impl : std::false_type {};

template<> struct has_enharmonic_key_impl<keys::BMaj> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::FsMaj> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::CsMaj> : std::true_type {};

template<> struct has_enharmonic_key_impl<keys::Gsmin> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::Dsmin> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::Asmin> : std::true_type {};

} // detail


template<class Key>
struct has_enharmonic_key
    : std::conditional_t<
        detail::has_enharmonic_key_impl<Key>::value ||
        detail::has_enharmonic_key_impl<cvt::detail::force_enharmonic_key_t<Key>>::value,
        std::true_type,
        std::false_type
    >
{};

template<class Key>
constexpr bool has_enharmonic_key_v = has_enharmonic_key<Key>::value;


template<class KeyFeel> struct is_major_feel;
template<> struct is_major_feel<key_feels::major> : std::true_type {};
template<> struct is_major_feel<key_feels::minor> : std::false_type {};
template<class KeyFeel> constexpr auto is_major_feel_v = is_major_feel<KeyFeel>::value;

template<class KeyFeel> struct is_minor_feel;
template<> struct is_minor_feel<key_feels::major> : std::false_type {};
template<> struct is_minor_feel<key_feels::minor> : std::true_type {};
template<class KeyFeel> constexpr auto is_minor_feel_v = is_minor_feel<KeyFeel>::value;


namespace cvt {

template<class Key, class Enabled = void>
struct enharmonic_key { using type = void; };

template<class Ident, class KeyFeel>
struct enharmonic_key<
    basic_key<key_ident<Ident, KeyFeel>>,
    std::enable_if_t<
        has_enharmonic_key_v<basic_key<key_ident<Ident, KeyFeel>>>
    >
>
{
    using type = basic_key<
        key_ident<
            basic_ident<
                Ident::height,
                std::conditional_t<
                    Ident::sharped, mods::flat, mods::sharp
                >
            >,
            KeyFeel
        >
    >;

    static_assert(!std::is_same_v<type, basic_key<key_ident<Ident, KeyFeel>>>, "enharmonic key must not be the same as original key");
};

template<class Key>
using enharmonic_key_t = typename enharmonic_key<Key>::type;


template<class Key, class Enabled = void>
struct enharmonic_key_pair;

template<class Key>
struct enharmonic_key_pair<Key, std::enable_if_t<has_enharmonic_key_v<Key>>>
{
    using type = std::tuple<Key, enharmonic_key_t<Key>>;
};

template<class Key>
struct enharmonic_key_pair<Key, std::enable_if_t<!has_enharmonic_key_v<Key>>>
{
    using type = std::tuple<Key>;
};

template<class Key>
using enharmonic_key_pair_t = typename enharmonic_key_pair<Key>::type;


namespace detail {

template<class Ident, class KeyFeel>
struct relative_ident;

template<ident_height_type Height, class Mod>
struct relative_ident<
    basic_ident<Height, Mod>,
    key_feels::major
>
{
    using type = basic_ident<height_shift_v<Height, 9>, Mod>;
};
template<ident_height_type Height, class Mod>
struct relative_ident<
    basic_ident<Height, Mod>,
    key_feels::minor
>
{
    using type = basic_ident<height_shift_v<Height, -9>, Mod>;
};

template<class Ident, class KeyFeel>
using relative_ident_t = typename relative_ident<Ident, KeyFeel>::type;

} // cvt::detail


template<class KeyFeel>
struct opposite_key_feel;

template<>
struct opposite_key_feel<key_feels::major>
{
    using type = key_feels::minor;
};
template<>
struct opposite_key_feel<key_feels::minor>
{
    using type = key_feels::major;
};
template<class KeyFeel>
using opposite_key_feel_t = typename opposite_key_feel<KeyFeel>::type;


template<class Key>
struct relative_key;

template<class Ident, class KeyFeel>
struct relative_key<basic_key<key_ident<Ident, KeyFeel>>>
{
    using type = basic_key<
        key_ident<
            detail::relative_ident_t<Ident, KeyFeel>,
            opposite_key_feel_t<KeyFeel>
        >
    >;
};

template<class Key>
using relative_key_t = typename relative_key<Key>::type;


template<class KeyFeel, class FeelMod>
struct modded_feel;

template<class KeyFeel>
struct modded_feel<
    KeyFeel,
    feel_mods::same
>
{
    using type = KeyFeel;
};

template<>
struct modded_feel<
    key_feels::major,
    feel_mods::opposite
>
{
    using type = key_feels::minor;
};

template<>
struct modded_feel<
    key_feels::minor,
    feel_mods::opposite
>
{
    using type = key_feels::major;
};

template<class KeyFeel, class FixedKeyFeel>
struct modded_feel<
    KeyFeel,
    feel_mods::forced_fixed<FixedKeyFeel>
>
{
    using type = FixedKeyFeel;
};

template<class KeyFeel, class FeelMod>
using modded_feel_t = typename modded_feel<KeyFeel, FeelMod>::type;


template<class HeightSet, class ModSet>
struct pack_to_ident_set;

template<ident_height_type... Heights, class... Mods>
struct pack_to_ident_set<
    height_set<Heights...>,
    mod_set<Mods...>
>
{
    using type = ident_set<basic_ident<Heights, Mods>...>;
};

template<class HeightSet, class ModSet>
using pack_to_ident_set_t = typename pack_to_ident_set<HeightSet, ModSet>::type;

} // cvt


template<class ScaleLike, class Enabled = void>
struct is_scale : std::false_type {};

template<
    class... Ts
>
struct is_scale<dynamic_scale<Ts...>> : std::true_type {};

template<
    class... Ts
>
struct is_scale<
    basic_scale<Ts...>
> : std::true_type {};

template<class ScaleLike>
constexpr auto is_scale_v = is_scale<ScaleLike>::value;


template<class Degree>
struct is_diatonic : std::false_type {};

template<class Degree>
constexpr bool is_diatonic_v = is_diatonic<Degree>::value;

template<> struct is_diatonic<basic_degree<0>> : std::true_type {};
template<> struct is_diatonic<basic_degree<2>> : std::true_type {};
template<> struct is_diatonic<basic_degree<4>> : std::true_type {};
template<> struct is_diatonic<basic_degree<5>> : std::true_type {};
template<> struct is_diatonic<basic_degree<7>> : std::true_type {};
template<> struct is_diatonic<basic_degree<9>> : std::true_type {};
template<> struct is_diatonic<basic_degree<11>> : std::true_type {};

}} // ompu
