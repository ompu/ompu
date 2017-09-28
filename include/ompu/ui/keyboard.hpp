#pragma once

#include "ompu/ui/component.hpp"
#include "ompu/ui/key_status.hpp"
#include "ompu/ui/synth/wheel.hpp"

#include "ompu/midi/spec.hpp"

#include "ompu/geo/ratio.hpp"

#include "saya/zed/fold.hpp"

#include <boost/range/adaptor/indexed.hpp>
#include <boost/assert.hpp>

#include <array>
#include <ratio>
#include <type_traits>


namespace ompu { namespace ui {

namespace keyboards {

template<class Kb, std::size_t I>
constexpr auto
to_midi_index_v = Kb::OffsetFromCneg1ToLowestKey + I;

template<class Kb, std::size_t I>
constexpr auto
key_white_keys_before_v = midi::key_white_keys_before_v<to_midi_index_v<Kb, I>> - Kb::IgnoredWhiteKeysBeforeLowest;

template<class Kb, std::size_t I>
using white_key_x_ratio_t = std::ratio_add<
    std::ratio_multiply<
        typename Kb::white_key_ratio::w_ratio,
        std::ratio<key_white_keys_before_v<Kb, I>, 1>
    >,
    std::ratio_multiply<
        typename Kb::clearance_ratio::w_ratio,
        std::ratio<key_white_keys_before_v<Kb, I>, 1>
    >
>;

namespace detail {

template<class Kb, std::size_t I, std::size_t OctaveI, class Enable = void>
struct black_key_x_ratio_impl;

template<class Kb, std::size_t I, std::size_t OctaveI>
struct black_key_x_ratio_impl<Kb, I, OctaveI, std::enable_if_t<OctaveI == 1 || OctaveI == 6>>
{
    using type = std::ratio_subtract<
        white_key_x_ratio_t<Kb, I>,
        std::ratio_multiply<
            typename Kb::white_key_ratio::w_ratio,
            std::ratio<40, 100>
        >
    >;
};

template<class Kb, std::size_t I, std::size_t OctaveI>
struct black_key_x_ratio_impl<Kb, I, OctaveI, std::enable_if_t<OctaveI == 3 || OctaveI == 10>>
{
    using type = std::ratio_subtract<
        std::ratio_subtract<
            std::ratio_add<
                white_key_x_ratio_t<Kb, I>,
                std::ratio_multiply<
                    typename Kb::white_key_ratio::w_ratio,
                    std::ratio<40, 100>
                >
            >,
            typename Kb::black_key_ratio::w_ratio
        >,
        typename Kb::clearance_ratio::w_ratio
    >;
};

template<class Kb, std::size_t I, std::size_t OctaveI>
struct black_key_x_ratio_impl<Kb, I, OctaveI, std::enable_if_t<OctaveI == 8>>
{
    using type = std::ratio_subtract<
        white_key_x_ratio_t<Kb, I>,
        std::ratio_multiply<
            typename Kb::black_key_ratio::w_ratio,
            std::ratio<1, 2>
        >
    >;
};

} // detail

template<class Kb, std::size_t I>
using black_key_x_ratio_t = typename detail::black_key_x_ratio_impl<Kb, I, midi::key_octave_index_v<to_midi_index_v<Kb, I>>>::type;


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


namespace detail {

template<std::size_t... LWs, std::size_t... LBs, std::size_t... HWs, std::size_t... Octave>
inline constexpr auto make_white_key_sequence_impl(
    std::index_sequence<LWs...>,
    std::index_sequence<LBs...>,
    std::index_sequence<Octave...>,
    std::index_sequence<HWs...>
)
{
    return saya::zed::make_seq_concat(
        std::index_sequence<LWs...>{},
        saya::zed::make_seq_offset<std::size_t, Octave * 12 + sizeof...(LWs)+sizeof...(LBs)>(midi::octave_white_key_sequence{})...,
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
        saya::zed::make_seq_offset<std::size_t, Octave * 12 + sizeof...(LWs)+sizeof...(LBs)>(midi::octave_black_key_sequence{})...
    );
}

} // detail

template<class Kb>
inline constexpr auto make_white_key_sequence()
{
    return detail::make_white_key_sequence_impl(
        Kb::lower_white_key_config_v,
        Kb::lower_black_key_config_v,
        midi::make_octave_sequence<Kb::Octaves>{},
        Kb::higher_white_key_config_v
    );
}

template<class Kb>
inline constexpr auto make_black_key_sequence()
{
    return detail::make_black_key_sequence_impl(
        Kb::lower_white_key_config_v,
        Kb::lower_black_key_config_v,
        midi::make_octave_sequence<Kb::Octaves>{},
        Kb::higher_white_key_config_v
    );
}

template<class Kb>
using make_clearance_sequence = std::make_index_sequence<Kb::ClearanceCount>;

} // keyboards

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
    using keyboard_type = BasicKeyboard<
        Octaves, LowerWhiteKeyConfig, LowerBlackKeyConfig, HigherWhiteKeyConfig, Keys, WhiteKeys, BlackKeys, ClearanceCount, OffsetFromCneg1ToLowestKey, KeyboardRatio, ClearanceRatio, WhiteKeyRatio, BlackKeyRatio, OctaveRatio
    >;

    using midi_spec = midi::Spec;

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
    static_assert(OffsetFromCneg1ToLowestKey <= midi_spec::NoteMax, "OffsetFromCneg1ToLowestKey <= midi_spec::NoteMax; You have configured a keyboard larger than MIDI constraints!!");

    static constexpr std::size_t const LowestKeyInMIDI = OffsetFromCneg1ToLowestKey;
    static constexpr std::size_t const HighestKeyInMIDI = LowestKeyInMIDI + Keys;

    static_assert(LowestKeyInMIDI < HighestKeyInMIDI, "lowest key < highest key");

    static_assert(LowestKeyInMIDI <= midi_spec::NoteMax, "lowest key should be less than midi_spec::NoteMax");
    static_assert(HighestKeyInMIDI <= midi_spec::NoteMax, "highest key should be less than midi_spec::NoteMax");

    static constexpr std::size_t const IgnoredKeysBeforeLowest = LowestKeyInMIDI;
    static constexpr std::size_t const IgnoredWhiteKeysBeforeLowest = midi::key_white_keys_before_v<LowestKeyInMIDI>;
    static constexpr std::size_t const IgnoredBlackKeysBeforeLowest = IgnoredKeysBeforeLowest - IgnoredWhiteKeysBeforeLowest;

    static constexpr std::size_t const IgnoredKeysAfterHighest = midi_spec::NoteMax - HighestKeyInMIDI;
    static constexpr std::size_t const IgnoredWhiteKeysAfterHighest = midi_spec::WhiteKeys - midi::key_white_keys_before_v<HighestKeyInMIDI + 1>;
    static constexpr std::size_t const IgnoredBlackKeysAfterHighest = IgnoredKeysAfterHighest - IgnoredWhiteKeysAfterHighest;

    //
    // distance from the lowest key on this config to the possible lowest `C`
    // in case of A, index to C == 3
    //
    static constexpr std::size_t OffsetFromLowestKeyToC =
        (12 - midi::key_octave_index_v<OffsetFromCneg1ToLowestKey>) % 12;

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
