#include "ompu/io.hpp"
#include "ompu/version.hpp"
#include "ompu/component_id.hpp"

#include <iostream>


namespace ompu {

std::ostream& operator<<(std::ostream& os, Version const& v)
{
    return os << v.str();
}

std::ostream& operator<<(std::ostream& os, ComponentID const& v)
{
    return os << v.id();
}

} // ompu
