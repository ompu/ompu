#pragma once

#include "ompu/ui_fwd.hpp"

namespace ompu { namespace ui {

class KeyStatus
{
public:
    void press(bool flag) noexcept { is_pressed_ = flag; }
    bool is_pressed() const noexcept { return is_pressed_; }
    explicit inline operator bool() const noexcept { return this->is_pressed(); }

private:
    bool is_pressed_{false};
};

}} // ompu
