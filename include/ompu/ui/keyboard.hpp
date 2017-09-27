#pragma once

#include "ompu/ui/component.hpp"
#include "ompu/ui/key_status.hpp"
#include "ompu/ui/synth/wheel.hpp"

#include "ompu/geo/ratio.hpp"

#include <boost/range/adaptor/indexed.hpp>
#include <boost/assert.hpp>

#include <array>
#include <ratio>
#include <type_traits>


namespace ompu { namespace ui {

template<
    std::size_t Octaves = 7, /* fully expressive octave, usually 1 smaller than actual octave */
    std::size_t Keys = Octaves * 12 + 3 /* lower */ + 1 /* upper */,
    std::size_t WhiteKeys = Octaves * 7 + 2 /* lower */ + 1 /* upper */,
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
    static constexpr std::size_t const Keys = Keys;
    static_assert(Keys >= Octaves * 12, "Keyboard must contain at least (Octaves * 12) keys");

    static constexpr std::size_t const OffsetFromCneg1ToLowestKey = OffsetFromCneg1ToLowestKey;
    static_assert(OffsetFromCneg1ToLowestKey <= 127, "OffsetFromCneg1ToLowestKey <= 127; You have configured a keyboard larger than MIDI constraints!!");

    template<class T>
    static constexpr auto to_midi_index(T key_idx) noexcept
    {
        return key_idx + OffsetFromCneg1ToLowestKey;
    }

    static constexpr std::size_t
    key_nth_octave(std::size_t const key_idx) noexcept
    {
        return to_midi_index(key_idx) / 12;
    }

    static constexpr std::size_t
    key_octave_index(std::size_t const key_idx) noexcept
    {
        return to_midi_index(key_idx) - key_nth_octave(key_idx) * 12;
    }


    //
    // distance from the lowest key on this config to the possible lowest `C`
    // in case of A, index to C == 3
    //
    static constexpr std::size_t OffsetFromLowestKeyToC =
        (12 - key_octave_index(OffsetFromCneg1ToLowestKey)) % 12;

    static_assert(OffsetFromLowestKeyToC <= 11, "[BUG] auto-detection for OffsetFromLowestKeyToC failed");

    using clearance_ratio = ClearanceRatio;

    static constexpr std::size_t const
    ClearanceCount = ClearanceCount;

    using clearance_sequence = std::make_index_sequence<ClearanceCount>;

    using keyboard_ratio = KeyboardRatio;

    using octave_ratio = OctaveRatio;
    using octave_sequence = std::make_index_sequence<Octaves>;

    using white_key_ratio = WhiteKeyRatio;
    using white_key_sequence = std::make_index_sequence<WhiteKeys>;

    using black_key_ratio = BlackKeyRatio;
    using black_key_sequence = std::make_index_sequence<BlackKeys>;

    // ------------------------------
    template<std::size_t Index>
    using clearance_x_ratio = std::ratio_add<
        std::ratio_multiply<
            typename white_key_ratio::w_ratio,
            std::ratio<Index + 1, 1>
        >,
        std::ratio_multiply<
            typename clearance_ratio::w_ratio,
            std::ratio<Index, 1>
        >
    >;

    // C#, F#
    using black_key_lefted_x_ratio = std::ratio_multiply<
        typename white_key_ratio::w_ratio,
        std::ratio<50, 100>
    >;

    // G#
    using black_key_centered_x_ratio = std::ratio_subtract<
        typename white_key_ratio::w_ratio,
        std::ratio_multiply<
            typename black_key_ratio::w_ratio,
            std::ratio<1, 2>
        >
    >;

    // D#, A#
    using black_key_righted_x_ratio = std::ratio_subtract<
        std::ratio_multiply<
            typename white_key_ratio::w_ratio,
            std::ratio<2, 1>
        >,
        typename black_key_ratio::w_ratio
    >;

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

    static constexpr std::size_t
    key_last_white_before(std::size_t const key_idx) noexcept
    {
        switch (key_octave_index(key_idx)) {
        case 1: case 3: case 6: case 8: case 10:
            return key_idx - 1;

        default:
            return key_idx;
        }
    }

    static constexpr std::size_t
    key_white_keys_before(std::size_t const key_idx) noexcept
    {
        return
            (key_nth_octave(key_idx) * 7) +
            key_octave_index(key_last_white_before(key_idx))
        ;
    }

    static constexpr bool
    key_is_white(std::size_t const key_idx) noexcept
    {
        switch (key_octave_index(key_idx)) {
        case 1: case 3: case 6: case 8: case 10:
            return false;

        default:
            return true;
        }
    }

    using base_type::base_type;

// private:
    PitchWheel pitch_w;
    ModWheel mod_w;

    using keys_type = std::array<KeyStatus, Keys>;
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

private:
    keys_type keys_;
};

using Keyboard = BasicKeyboard<>;

}} // ompu
