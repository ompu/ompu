#pragma once

#include "saya/zed/fold.hpp"
#include "saya/zed/seq.hpp"

#include <type_traits>
#include <utility>


namespace ompu { namespace midi {

template<std::size_t I>
constexpr std::size_t
key_nth_octave_v = I / 12;

template<std::size_t I>
constexpr std::size_t
key_octave_index_v = I - key_nth_octave_v<I> * 12;


namespace detail {

template<std::size_t I> struct key_is_white_impl : std::true_type {};
template<> struct key_is_white_impl<1> : std::false_type {};
template<> struct key_is_white_impl<3> : std::false_type {};
template<> struct key_is_white_impl<6> : std::false_type {};
template<> struct key_is_white_impl<8> : std::false_type {};
template<> struct key_is_white_impl<10> : std::false_type {};

} // detail

template<std::size_t I>
constexpr auto key_is_white_v = detail::key_is_white_impl<key_octave_index_v<I>>::value;


namespace detail {

template<std::size_t I, class Enabled = void>
struct key_last_white_before_impl;

template<std::size_t I>
struct key_last_white_before_impl<I, std::enable_if_t<key_is_white_v<I>>>
{
    static constexpr std::size_t value = I;
};

template<std::size_t I>
struct key_last_white_before_impl<I, std::enable_if_t<!key_is_white_v<I>>>
{
    static constexpr std::size_t value = I - 1;
};

} // detail

template<std::size_t I>
constexpr auto key_last_white_before_v = detail::key_last_white_before_impl<I>::value;

namespace detail {

template<std::size_t I> struct white_key_count_in_octave;
template<> struct white_key_count_in_octave<0> : std::integral_constant<std::size_t, 0> {};
template<> struct white_key_count_in_octave<1> : std::integral_constant<std::size_t, 1> {};
template<> struct white_key_count_in_octave<2> : std::integral_constant<std::size_t, 1> {};
template<> struct white_key_count_in_octave<3> : std::integral_constant<std::size_t, 2> {};
template<> struct white_key_count_in_octave<4> : std::integral_constant<std::size_t, 2> {};
template<> struct white_key_count_in_octave<5> : std::integral_constant<std::size_t, 3> {};
template<> struct white_key_count_in_octave<6> : std::integral_constant<std::size_t, 4> {};
template<> struct white_key_count_in_octave<7> : std::integral_constant<std::size_t, 4> {};
template<> struct white_key_count_in_octave<8> : std::integral_constant<std::size_t, 5> {};
template<> struct white_key_count_in_octave<9> : std::integral_constant<std::size_t, 5> {};
template<> struct white_key_count_in_octave<10> : std::integral_constant<std::size_t, 6> {};
template<> struct white_key_count_in_octave<11> : std::integral_constant<std::size_t, 6> {};

template<std::size_t I>
constexpr auto white_key_count_in_octave_v = white_key_count_in_octave<I>::value;

} // detail

template<std::size_t I>
constexpr auto
key_white_keys_before_v =
    (key_nth_octave_v<I> * 7) +
    detail::white_key_count_in_octave_v<key_octave_index_v<I>>
;


template<std::size_t Octaves>
using make_octave_sequence = std::make_index_sequence<Octaves>;

using octave_white_key_sequence = std::index_sequence<
    0, 2, 4, 5, 7, 9, 11
>;

namespace detail {

template<class...>
struct white_key_sequence_impl;

template<std::size_t Octave, class Seq>
using white_key_sequence_impl_msvc_workaround = typename saya::zed::i_seq_offset<std::size_t, Octave * 12, Seq>::type;

template<std::size_t... Octaves, class WhiteKeySeq>
struct white_key_sequence_impl<std::index_sequence<Octaves...>, WhiteKeySeq>
{
    using type = saya::zed::i_seq_concat_t<
        white_key_sequence_impl_msvc_workaround<Octaves, WhiteKeySeq>...
    >;
};

} // detail

template<std::size_t Octaves>
using make_white_key_sequence = typename detail::white_key_sequence_impl<make_octave_sequence<Octaves>, octave_white_key_sequence>::type;

using octave_black_key_sequence = std::index_sequence<
    1, 3, 6, 8, 10
>;

class Spec
{
public:
    static constexpr std::size_t ChannelMax = 16;
    static constexpr std::size_t NoteMin = 0;
    static constexpr std::size_t NoteMax = 127;

    static constexpr auto Keys = NoteMax + 1;

    static constexpr std::size_t C0 = NoteMin;
    static constexpr std::size_t A4 = 69;

    static constexpr auto WhiteKeys = key_white_keys_before_v<NoteMax + 1>;
    static constexpr auto BlackKeys = Keys - WhiteKeys;

    static constexpr std::size_t Octaves = 10;
    //static_assert(Octaves == 10, "Octaves must be 10");

    static_assert(Keys == 128, "Keys must be 128");
    static_assert(WhiteKeys == 7 * Octaves + 5, "WhiteKeys must be 75");
    static_assert(BlackKeys == 5 * Octaves + 3, "BlackKeys must be 53");

    using white_key_sequence = saya::zed::i_seq_concat_t<
        make_white_key_sequence<Octaves>, std::index_sequence<120, 122, 124, 125, 127>
    >;
    static_assert(
        std::is_same_v<
            white_key_sequence,
            std::index_sequence<
                0, 2, 4, 5, 7, 9, 11,
                12, 14, 16, 17, 19, 21, 23,
                24, 26, 28, 29, 31, 33, 35,
                36, 38, 40, 41, 43, 45, 47,
                48, 50, 52, 53, 55, 57, 59,
                60, 62, 64, 65, 67, 69, 71,
                72, 74, 76, 77, 79, 81, 83,
                84, 86, 88, 89, 91, 93, 95,
                96, 98, 100, 101, 103, 105, 107,
                108, 110, 112, 113, 115, 117, 119,
                120, 122, 124, 125, 127
            >
        >,
        "white key sequence must be sane"
    );
};

}} // ompu
