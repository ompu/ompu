#include "ompu/plug/game/drawer.hpp"

#include <boost/core/ignore_unused.hpp>


namespace ompu { namespace plug { namespace game {

void Drawer::partial(ompu::ui::ModWheel const& c) const
{
    boost::ignore_unused(c);
}

void Drawer::partial(ompu::ui::PitchWheel const& c) const
{
    boost::ignore_unused(c);
}

}}} // ompu
