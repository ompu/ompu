#pragma once

#include "ompu/music/tone_types.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

template<class ToneHeight>
struct basic_tone
{
    using height_type = ToneHeight;
    static constexpr auto height = ToneHeight::value;
};

template<class... Tones>
struct tone_set;

template<class... ToneHeights>
struct tone_set<basic_tone<ToneHeights>...>
{
    static_assert(
        saya::zed::all_of_v<
            saya::zed::is_true,
            saya::zed::template_<>,
            saya::zed::reduced_t<
                saya::zed::not_eq,
                saya::zed::template_<>,
                ToneHeights...
            >
        >,
        "all tones must not have the same height as the previous tone"
    );

    static constexpr std::size_t count = sizeof...(ToneHeights);
};

}} // ompu
