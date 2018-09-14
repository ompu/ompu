#pragma once

#include "ompu/plug/app_fwd.hpp"
#include "ompu/ui/all.hpp"

#include <memory>


namespace ompu { namespace plug { namespace game {

class UISet
{
public:
    UISet() = default;
    ~UISet() = default;

    void init(App* app);

    UISet(UISet const&) = delete;
    UISet(UISet&&) = delete;

    UISet& operator=(UISet const&) = delete;
    UISet& operator=(UISet&&) = delete;

    ompu::ui::Keyboard
    keyboard;
};

}}} // ompu
