#include "ompu/ompu_lib.hpp"

namespace ompu { namespace ompu_lib {

Version const& version()
{
    static Version const v_{1, 0, 0, "alpha"};
    return v_;
}

}} // ompu
