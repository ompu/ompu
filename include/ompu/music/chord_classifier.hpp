#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/type_traits.hpp"
#include "ompu/music/key.hpp"
#include "ompu/music/key_ident.hpp"
#include "ompu/music/scale.hpp"
#include "ompu/music/chord.hpp"


namespace ompu { namespace music {

struct chord_classifier
{
    template<class FundSet, class TensionSet>
    constexpr chord_classifier(basic_chord<FundSet, TensionSet> const&) noexcept
        : is_power_chord(is_power_chord_v<FundSet, TensionSet>)
        , is_seventh(is_seventh_chord_v<FundSet>)
        , is_major(!is_power_chord && is_major_v<FundSet>)
    {}

    ~chord_classifier() noexcept = default;

    template<class KeyIdent, class ToneSet>
    static void translate_on_key(KeyIdent, ToneSet const& tones)
    {
        // TODO
#if 0
        for (auto const& tone : tones) {

        }
#endif
    }

    static constexpr auto name = sprout::to_string("aaaaaaaaaaaaAAAAaaaAAAAA!!!!!!!!!!!!!!!");

    bool is_power_chord = false;
    bool is_seventh = false;
    bool is_major = false;
    bool is_diatonic = false;
};

template<class KeyIdent>
inline auto chord_class_to_chord(chord_classifier const& cc) noexcept
{
    namespace cns = chord_notes;

    if (cc.is_power_chord) {
        return basic_chord<chord_fund_set<cns::P5>, chord_tension_set<>>{};
    }

    return basic_chord<void, void>{};
}

}} // ompu
