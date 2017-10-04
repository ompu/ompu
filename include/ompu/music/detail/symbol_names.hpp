#pragma once

namespace ompu { namespace music {

namespace detail {

struct symbol_names
{
    static constexpr auto empty = sprout::to_string("");
    static constexpr auto sharp = sprout::to_string(u8"\u266f");
    static constexpr auto flat = sprout::to_string(u8"\u266d");
    static constexpr auto natural = sprout::to_string(u8"\u266e");

    template<class Str>
    static constexpr auto to_related(Str const& str) { return sprout::to_string("[") + str + sprout::to_string("]"); }

    static constexpr auto tonic = sprout::to_string("T");
    static constexpr auto tonic_r = to_related(tonic);
    static constexpr auto dominant = sprout::to_string("D");
    static constexpr auto dominant_r = to_related(dominant);
    static constexpr auto sub_dominant = sprout::to_string("SD");
    static constexpr auto sub_dominant_r = to_related(sub_dominant);

    static constexpr auto parallel = sprout::to_string("Parallel");
    static constexpr auto parallel_r = to_related(parallel);
    static constexpr auto relative = sprout::to_string("Relative");
    static constexpr auto relative_r = to_related(relative);
};

} // detail

}} // ompu
