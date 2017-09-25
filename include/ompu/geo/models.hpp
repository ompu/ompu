#pragma once

#include "ompu/geo/geo.hpp"
#include "ompu/geo/strats.hpp"
#include "ompu/geo/idents.hpp"


namespace ompu { namespace geo { namespace models {

struct Box : idents::Box
{
    using strats_type = strats::Box;
    using ident_type = idents::Box;

    using ident_type::ident_type;
};

struct Circle : idents::Circle
{
    using strats_type = strats::Circle;
    using ident_type = idents::Circle;

    using ident_type::ident_type;
};

}}} // ompu
