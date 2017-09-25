#pragma once

#include "ompu/ui/component.hpp"
#include "ompu/ui/synth/wheel.hpp"


namespace ompu { namespace ui {

class Keyboard : public Component<Keyboard, geo::models::Box>
{
public:
    template<class Visitor>
    friend class Updatable<Visitor>;

    template<class T>
    friend class Drawable<Visitor>;

    static constexpr char const* const component_name() { return "Keyboard"; }

    using base_type::base_type;

private:
    PitchWheel pitch_w_;
    ModWheel mod_w_;
};

}} // ompu
