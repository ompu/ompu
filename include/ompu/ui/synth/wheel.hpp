#pragma once

#include "ompu/ui/component.hpp"


namespace ompu { namespace ui {

template<class Tag, class T, T Min, T Max>
class SynthWheel
    : public Component<geo::models::Box>
    , tags::synth_wheel_base
{
public:
    static constexpr double Step = double(Max - Min) / 100.;

    static_assert(Min <= Max, "Min <= Max");
    static_assert(Step > 0, "Step > 0");
    static constexpr char const* const component_name() { return "SynthWheel"; }

    using tag_type = Tag;
    using value_type = T;
    using base_type::base_type;

    value_type const& val() const noexcept { return val_; }

    void set_val(value_type v)
    {
        if (!(Min <= v && v <= Max)) {
            throw std::out_of_range("invalid value given to SynthWheel::set_val()");
        }
        val_ = std::move(v);
    }

    double pct() const noexcept
    {
        return (val_ - Min) * Step;
    }

private:
    T val_;
};

}} // ompu
