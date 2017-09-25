#pragma once

#include "ompu/geo/geo.hpp"


namespace ompu { namespace geo { namespace strats {

class Box
{
public:

};

class Circle
{
public:
    static bg::strategy::buffer::point_circle const
    point;

    static bg::strategy::buffer::join_round const
    join;

    static bg::strategy::buffer::end_round const
    end;

    static bg::strategy::buffer::side_straight const
    side;
};

}}} // strats
