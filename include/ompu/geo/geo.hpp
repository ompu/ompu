#pragma once

#include <boost/geometry/core/access.hpp>

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/geometry/geometries/multi_point.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>

#include <boost/geometry/strategies/strategies.hpp>


namespace ompu { namespace geo {

namespace bg = boost::geometry;

using Point = bg::model::d2::point_xy<double>;
using MultiPoint = bg::model::multi_point<Point>;

using Polygon = bg::model::polygon<Point>;
using MultiPolygon = bg::model::multi_polygon<Polygon>;
using Box = bg::model::box<Point>;

struct Size
{
    unsigned w, h;
};

struct CircleSize
{
    bg::strategy::buffer::distance_symmetric<double>
    radius;
};

}} // ompu

