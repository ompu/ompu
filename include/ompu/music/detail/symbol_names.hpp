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

template<unsigned N> struct number_names { static constexpr auto name = sprout::to_string("(empty-name)"); };
// template<> struct number_names<0> { static constexpr auto name = sprout::to_string("0"); };
// template<> struct number_names<1> { static constexpr auto name = sprout::to_string("0"); };
// template<> struct number_names<2> { static constexpr auto name = sprout::to_string("0"); };
// template<> struct number_names<3> { static constexpr auto name = sprout::to_string("0"); };
// template<> struct number_names<4> { static constexpr auto name = sprout::to_string("0"); };
template<> struct number_names<5> { static constexpr auto name = sprout::to_string("5"); };
template<> struct number_names<6> { static constexpr auto name = sprout::to_string("6"); };
template<> struct number_names<7> { static constexpr auto name = sprout::to_string("7"); };
// template<> struct number_names<8> { static constexpr auto name = sprout::to_string("0"); };
template<> struct number_names<9> { static constexpr auto name = sprout::to_string("9"); };
template<> struct number_names<10> { static constexpr auto name = sprout::to_string("10"); };
template<> struct number_names<11> { static constexpr auto name = sprout::to_string("11"); };
// template<> struct number_names<12> { static constexpr auto name = sprout::to_string("0"); };
template<> struct number_names<13> { static constexpr auto name = sprout::to_string("13"); };

} // detail

}} // ompu
