#include "ompu/geo/idents.hpp"


namespace ompu { namespace geo { namespace idents {

Box::Box(geo::Point pt, geo::Size size) noexcept
    : pt(std::move(pt))
    , size(std::move(size))
{
    this->cache_ident();
}

void Box::cache_ident()
{
    cache_.min_corner() = pt;
    cache_.max_corner() = geo::Point{bg::get<0>(pt) + size.w, bg::get<1>(pt) + size.h};
}

double Box::x() const noexcept
{
    return bg::get<0>(cache_.min_corner());
}

double Box::y() const noexcept
{
    return bg::get<1>(cache_.min_corner());
}


Circle::Circle(geo::Point pt, geo::CircleSize size) noexcept
    : pt(std::move(pt))
    , size(std::move(size))
{
    this->cache_ident();
}

void Circle::cache_ident()
{
    // ...
}

double Circle::x() const noexcept
{
    return bg::get<0>(pt);
}

double Circle::y() const noexcept
{
    return bg::get<1>(pt);
}


}}} // ompu
