#pragma once

#include "ompu/ompu_fwd.hpp"

#include <string>
#include <iosfwd>


namespace ompu {

namespace io {

using in_iterator_type = std::string::const_iterator;

class reader;
class writer;

} // io

std::ostream& operator<<(std::ostream& os, Version const& v);
std::ostream& operator<<(std::ostream& os, ComponentID const& v);

} // ompu
