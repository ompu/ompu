#pragma once

#include "ompu/ui_fwd.hpp"
#include "ompu/ui/keyboard.hpp"

#include <boost/variant/variant.hpp>
#include <memory>


namespace ompu { namespace ui {

namespace detail {

template<class... Ts>
using variant_of_shared_ptr =
    boost::variant<std::shared_ptr<Ts>...>
;

} // detail

using any_component = detail::variant_of_shared_ptr<
      PitchWheel
    , ModWheel
    , Keyboard
>;

}} // ompu
