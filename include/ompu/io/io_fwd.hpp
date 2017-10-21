#pragma once

#include "ompu/ompu_fwd.hpp"

#include <iosfwd>


namespace ompu {

class reader;
class writer;

std::ostream& operator<<(std::ostream& os, Version const& v);
std::ostream& operator<<(std::ostream& os, ComponentID const& v);

} // ompu
