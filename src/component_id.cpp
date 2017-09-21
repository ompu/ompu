#include "ompu/component_id.hpp"

namespace ompu {

bool operator==(ComponentID const& lhs, ComponentID const& rhs)
{
    return lhs.id() == rhs.id();
}

} // ompu

