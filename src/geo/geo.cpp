#include "ompu/geo.hpp"


namespace ompu { namespace geo {

namespace strats {

bg::strategy::buffer::point_circle const
Circle::point{360};

bg::strategy::buffer::join_round const
Circle::join;

bg::strategy::buffer::end_round const
Circle::end;

bg::strategy::buffer::side_straight const
Circle::side;

} // strats

}} // ompu

