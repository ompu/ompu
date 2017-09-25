#pragma once

#include "ompu/ui/component.hpp"


namespace ompu { namespace ui {

namespace tags {

struct synth_wheel_base {};

struct pitch_wheel {};
struct mod_wheel {};

} // tags

template<class Tag, class T, T Min, T Max>
class SynthWheel
    : public Component<SynthWheel, geo::models::Box>
    , tags::synth_wheel_base
{
public:
    template<class Visitor>
    friend class Updatable<Visitor>;

    template<class T>
    friend class Drawable<Visitor>;

    static constexpr double Step = double(Max - Min) / 100.;

    static_assert(Min <= Max, "Min <= Max");
    static_assert(Step > 0, "Step > 0");
    static constexpr char const* const component_name() { return "SynthWheel"; }

    using tag_type = Tag;
    using value_type = T;
    using base_type::base_type;

    value_type const& val() const noexcept { return val_; }
    void set_val(value_type v) noexcept { val_ = std::move(v); }

    double pct() const noexcept
    {
        return (val_ - Min) * Step;
    }

private:
    T val_;
};

using PitchWheel = SynthWheel<tags::pitch_wheel, int, -8192, 8191>;
using ModWheel = SynthWheel<tags::mod_wheel, unsigned, 0, 127>;

}} // ompu
