#pragma once

#include "ompu/ui/component.hpp"
#include "ompu/ui/key_status.hpp"
#include "ompu/ui/synth/wheel.hpp"

#include "ompu/geo/ratio.hpp"

#include "saya/zed/fold.hpp"

#include <boost/range/adaptor/indexed.hpp>
#include <boost/assert.hpp>

#include <array>
#include <ratio>
#include <type_traits>


namespace ompu { namespace ui {

namespace keyboards {

template<std::size_t MIDIOfs, std::size_t I>
static constexpr auto
to_midi_index_v = MIDIOfs + I;

template<std::size_t MIDIOfs, std::size_t I>
static constexpr auto
key_nth_octave_v = to_midi_index_v<MIDIOfs, I> / 12;

template<std::size_t MIDIOfs, std::size_t I>
static constexpr auto
key_octave_index_v = to_midi_index_v<MIDIOfs, I> - key_nth_octave_v<MIDIOfs, I> * 12;


#if 0
    key_idx = 15
    note = D#
    is_white = false
    nth_octave = 1
    octave_index = 3
    last_white_before = 14
    white_keys_before = 8

    key_idx = 33
    note = A
    is_white = true
    nth_octave = 2
    octave_index = 9
    last_white_before = 33
    white_keys_before = 18
#endif


template<std::size_t MIDIOfs, std::size_t I>
static constexpr auto
key_last_white_before()
{
    switch (key_octave_index_v<MIDIOfs, I>) {
    case 1: case 3: case 6: case 8: case 10:
        return I - 1;

    default:
        return I;
    }
}

template<std::size_t MIDIOfs, std::size_t I>
static constexpr auto
key_white_keys_before_v =
    (key_nth_octave_v<MIDIOfs, I> * 7) +
    key_octave_index_v<MIDIOfs, key_last_white_before<MIDIOfs, I>()>
;

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


template<class Kb, std::size_t I>
using white_key_x_ratio_t = std::ratio_add<
    std::ratio_multiply<
        typename Kb::white_key_ratio::w_ratio,
        std::ratio<I, 1>
    >,
    std::ratio_multiply<
        typename Kb::clearance_ratio::w_ratio,
        std::ratio<I, 1>
    >
>;


template<class Kb, std::size_t ZeroI, std::size_t RealI, class Enable = void>
struct black_key_x_ratio;

template<class Kb, std::size_t ZeroI, std::size_t RealI>
struct black_key_x_ratio<Kb, ZeroI, RealI, std::enable_if_t<ZeroI % 5 == 0 || ZeroI % 5 == 2>>
{
    using type = std::ratio_subtract<
        white_key_x_ratio_t<Kb, key_white_keys_before_v<Kb::OffsetFromCneg1ToLowestKey, RealI>>,
        std::ratio_multiply<
            typename Kb::white_key_ratio::w_ratio,
            std::ratio<50, 100>
        >
    >;
};

template<class Kb, std::size_t ZeroI, std::size_t RealI>
struct black_key_x_ratio<Kb, ZeroI, RealI, std::enable_if_t<ZeroI % 5 == 1 || ZeroI % 5 == 4>>
{
    using type = std::ratio_subtract<
        white_key_x_ratio_t<Kb, key_white_keys_before_v<Kb::OffsetFromCneg1ToLowestKey, RealI>>,
        std::ratio_multiply<
            typename Kb::white_key_ratio::w_ratio,
            std::ratio<15, 100>
        >
    >;
};

template<class Kb, std::size_t ZeroI, std::size_t RealI>
struct black_key_x_ratio<Kb, ZeroI, RealI, std::enable_if_t<ZeroI % 5 == 3>>
{
    using type = std::ratio_subtract<
        white_key_x_ratio_t<Kb, key_white_keys_before_v<Kb::OffsetFromCneg1ToLowestKey, RealI>>,
        std::ratio_multiply<
            typename Kb::black_key_ratio::w_ratio,
            std::ratio<1, 2>
        >
    >;
};

template<class Kb, std::size_t ZeroI, std::size_t RealI>
using black_key_x_ratio_t = typename black_key_x_ratio<Kb, ZeroI, RealI>::type;


template<class Kb, std::size_t I>
using clearance_x_ratio_t = std::ratio_add<
    std::ratio_multiply<
        typename Kb::white_key_ratio::w_ratio,
        std::ratio<I + 1, 1>
    >,
    std::ratio_multiply<
        typename Kb::clearance_ratio::w_ratio,
        std::ratio<I, 1>
    >
>;

} // keyboards

template<std::size_t Octaves>
using make_octave_sequence = std::make_index_sequence<Octaves>;

using octave_white_key_sequence = std::index_sequence<
    0, 2, 4, 5, 7, 9, 11
>;

using octave_black_key_sequence = std::index_sequence<
    1, 3, 6, 8, 10
>;

namespace detail {

template<std::size_t... LWs, std::size_t... LBs, std::size_t... HWs, std::size_t... Octave>
inline constexpr auto make_white_key_sequence_impl(
    std::index_sequence<LWs...>,
    std::index_sequence<LBs...>,
    std::index_sequence<Octave...>,
    std::index_sequence<HWs...>
) {
    return saya::zed::make_seq_concat(
        std::index_sequence<LWs...>{},
        saya::zed::make_seq_offset<Octave * 12 + sizeof...(LWs) + sizeof...(LBs)>(octave_white_key_sequence{})...,
        std::index_sequence<HWs...>{}
    );
}

template<std::size_t... LWs, std::size_t... LBs, std::size_t... HWs, std::size_t... Octave>
inline constexpr auto make_black_key_sequence_impl(
    std::index_sequence<LWs...>,
    std::index_sequence<LBs...>,
    std::index_sequence<Octave...>,
    std::index_sequence<HWs...>
)
{
    return saya::zed::make_seq_concat(
        std::index_sequence<LBs...>{},
        saya::zed::make_seq_offset<Octave * 12 + sizeof...(LWs) + sizeof...(LBs)>(octave_black_key_sequence{})...
    );
}

} // detail

template<class Kb>
inline constexpr auto make_white_key_sequence()
{
    return detail::make_white_key_sequence_impl(
        Kb::lower_white_key_config_v,
        Kb::lower_black_key_config_v,
        make_octave_sequence<Kb::Octaves>{},
        Kb::higher_white_key_config_v
    );
}

template<class Kb>
inline constexpr auto make_black_key_sequence()
{
    return detail::make_black_key_sequence_impl(
        Kb::lower_white_key_config_v,
        Kb::lower_black_key_config_v,
        make_octave_sequence<Kb::Octaves>{},
        Kb::higher_white_key_config_v
    );
}

template<class Kb>
using make_clearance_sequence = std::make_index_sequence<Kb::ClearanceCount>;


// -------------------------------------------------


template<
    std::size_t Octaves = 7, /* fully expressive octave, usually 1 smaller than actual octave */
    class LowerWhiteKeyConfig = std::index_sequence<0, 2>,
    class LowerBlackKeyConfig = std::index_sequence<1>,
    class HigherWhiteKeyConfig = std::index_sequence<Octaves * 12 + 3 + 1 - 1>,
    std::size_t Keys = Octaves * 12 + LowerWhiteKeyConfig::size() + LowerBlackKeyConfig::size() + HigherWhiteKeyConfig::size(),
    std::size_t WhiteKeys = Octaves * 7 + LowerWhiteKeyConfig::size() + HigherWhiteKeyConfig::size(),
    std::size_t BlackKeys = Keys - WhiteKeys,
    std::size_t ClearanceCount = WhiteKeys - 1,

    // a.k.a. MIDI note C-1 == 0
    std::size_t OffsetFromCneg1ToLowestKey = 21, // C-1 = 0, A0 = 21

    //
    // Keyboard configuration
    //
    // NB: actual piano keys are crafted based on the *single octave width*
    //

    // 88-key full width = 1225mm
    class KeyboardRatio = geo::RatioSize<
        std::ratio<1, 1225>,
        std::ratio<1, 150>
    >,

    // Clearance = 1mm
    class ClearanceRatio = geo::RatioSize<
        typename KeyboardRatio::w_ratio,
        std::ratio<1, 1>
    >,

    // White = 23mm x 150mm
    class WhiteKeyRatio = geo::RatioSize<
        std::ratio_multiply<
            std::ratio_subtract<
                std::ratio<1, 1>,
                std::ratio_multiply<
                    typename ClearanceRatio::w_ratio,
                    std::ratio<ClearanceCount, 1>
                >
            >,
            std::ratio<1, WhiteKeys>
        >,
        std::ratio<1, 1>
    >,

    // Black = 11mm x 95mm
    class BlackKeyRatio = geo::RatioSize<
        std::ratio_multiply<std::ratio<11, 1>, typename KeyboardRatio::w_ratio>,
        std::ratio_multiply<std::ratio<95, 1>, typename KeyboardRatio::h_ratio>
    >,

    // 88-key octave width = 165mm
    class OctaveRatio = geo::RatioSize<
        std::ratio_add<
            std::ratio_multiply<std::ratio<7, 1>, typename WhiteKeyRatio::w_ratio>,
            std::ratio_multiply<std::ratio<6, 1>, typename ClearanceRatio::w_ratio>
        >,
        std::ratio<1, 1>
    >
>
class BasicKeyboard : public Component<geo::models::Box>
{
public:
    static constexpr char const* const component_name() { return "Keyboard"; }

    //
    // Keyboard standard size notes:
    //
    static constexpr std::size_t const Octaves = Octaves;
    static constexpr std::size_t const BlackKeys = BlackKeys;

    static constexpr std::size_t const WhiteKeys = WhiteKeys;

    static constexpr auto lower_white_key_config_v = LowerWhiteKeyConfig{};
    static constexpr auto lower_black_key_config_v = LowerBlackKeyConfig{};
    static constexpr auto higher_white_key_config_v = HigherWhiteKeyConfig{};

    static constexpr std::size_t const Keys = Keys;
    static_assert(Keys >= Octaves * 12, "Keyboard must contain at least (Octaves * 12) keys");

    static constexpr std::size_t const OffsetFromCneg1ToLowestKey = OffsetFromCneg1ToLowestKey;
    static_assert(OffsetFromCneg1ToLowestKey <= 127, "OffsetFromCneg1ToLowestKey <= 127; You have configured a keyboard larger than MIDI constraints!!");


    //
    // distance from the lowest key on this config to the possible lowest `C`
    // in case of A, index to C == 3
    //
    static constexpr std::size_t OffsetFromLowestKeyToC =
        (12 - keyboards::key_octave_index_v<OffsetFromCneg1ToLowestKey, OffsetFromCneg1ToLowestKey>) % 12;

    static_assert(OffsetFromLowestKeyToC <= 11, "[BUG] auto-detection for OffsetFromLowestKeyToC failed");

    using clearance_ratio = ClearanceRatio;

    static constexpr std::size_t const
    ClearanceCount = ClearanceCount;

    using keyboard_ratio = KeyboardRatio;
        using octave_ratio = OctaveRatio;

    // ------------------------------

    using white_key_ratio = WhiteKeyRatio;
    using black_key_ratio = BlackKeyRatio;

    // ------------------------------

    //
    // Sanity check...
    //
    static_assert(Keys >= Octaves * 12, "Key count must be greater or equal to (Octaves * 12)");
    static_assert(BlackKeys < WhiteKeys, "Black key count must be smaller than White keys");
    static_assert(BlackKeys + WhiteKeys == Keys, "White + Black must be total Key count");

    static_assert(
        std::ratio_equal_v<
            std::ratio_add<
                std::ratio_multiply<std::ratio<7, 1>, typename WhiteKeyRatio::w_ratio>,
                std::ratio_multiply<std::ratio<6, 1>, typename ClearanceRatio::w_ratio>
            >,
            typename OctaveRatio::w_ratio
        >,
        "(White[W] * 7 + Clearance[W] * 6) must be equal to Octave[W]"
    );

    static_assert(
        std::ratio_equal_v<
            std::ratio_add<
                std::ratio_multiply<typename WhiteKeyRatio::w_ratio, std::ratio<WhiteKeys, 1>>,
                std::ratio_multiply<typename ClearanceRatio::w_ratio, std::ratio<ClearanceCount, 1>>
            >,
            std::ratio<1, 1>
        >,
        "(White[W] * WhiteKeys + Clearance[W] * ClearanceCount) must be equal to keyboard fullwidth"
    );

    static_assert(
        std::ratio_less_v<
            typename BlackKeyRatio::w_ratio,
            typename WhiteKeyRatio::w_ratio
        >,
        "Black[W] must be smaller than White[W]"
    );

    static_assert(
        std::ratio_less_v<
            typename BlackKeyRatio::h_ratio,
            typename WhiteKeyRatio::h_ratio
        >,
        "Black[H] must be smaller than White[H]"
    );

    static_assert(
        std::ratio_less_equal_v<
            std::ratio_multiply<
                typename OctaveRatio::w_ratio,
                std::ratio<Octaves, 1>
            >,
            std::ratio<1, 1>
        >,
        "(Octave[W] * Octaves) must be smaller or equal to Keyboard[W]"
    );

    using base_type::base_type;
    using keys_type = std::array<KeyStatus, Keys>;

private:
    keys_type keys_;

public:
    template<std::size_t I>
    inline constexpr typename keys_type::const_reference
    nth_key() const noexcept { return keys_[I]; }

public:
    PitchWheel pitch_w;
    ModWheel mod_w;

    keys_type const& keys() const noexcept { return keys_; }
    keys_type& keys() noexcept { return keys_; }

    constexpr void press_at_midi(std::size_t note_num, bool press) noexcept
    {
        if (note_num < OffsetFromCneg1ToLowestKey) return;

        auto const real_key = note_num - OffsetFromCneg1ToLowestKey;
        if (real_key > Keys) return;

        keys_[real_key].press(press);
    }

    constexpr auto indexed_keys() const noexcept
    {
        return keys_ | boost::adaptor::indexed(0 /* OffsetFromCneg1ToLowestKey */);
    }
};

using Keyboard = BasicKeyboard<>;

}} // ompu
